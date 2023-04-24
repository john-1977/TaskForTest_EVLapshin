#include <iostream>
#include <list>
#include "MySocket.h"
#include <thread>
#include <chrono>
#include <string>
#include <shared_mutex>

#include <iCmd.h>
#include <memory>

#include <cmd_on.h>
#include <Cmd_Off.h>
#include <Cmd_SetColor.h>


//Набор данных для потоков
using namespace std::chrono_literals;
struct ThreadStatus
{
    ThreadStatus() : logic_id(-1), cmd (-1), executed(true), finished(false), need_finish(false) {}

    int logic_id; //логический номер потока (всегда идет на инкремент)
    BYTE cmd; // команда (тип)
    bool executed; // признак того что команда выполнена и клиент готов к новой команде
    bool finished; // признак того что клиент завершил свою работу (требуется удалить о нем запись из списка потоков)
    bool need_finish; // команда потоку на завершение
    std::shared_ptr<iCmd> data; //данные по команде
};
std::shared_mutex _my_mutex; //мьютекс для доступа к _threads_stat на изменение
std::list<ThreadStatus> _threads_stat; //список потоков, их состояний, передача потокам команд

int main(int argc, char *argv[]);

// Набор функций-обработчиков потоков
void client_thread(int logic_id, std::shared_ptr<ServerClnt> client_sock);
void create_client(int logic_id, std::shared_ptr<ServerClnt> client_sock); //порождает thread и заполняет необходимые статусы в списке состояний потоков (_threads_stat)
void remove_dead_clients_thread ();
void main_thread(std::shared_ptr<ServerSocket> server);

void ShowCaption ();
void CMD_SaveCmd (int logic_id, std::shared_ptr<iCmd> cmd); //записывает команду клиенту или клиентам
void CMD_Flashligh_Kill(int logic_id = -1); //убить клиентский поток



int main (int argc, char* argv[])
{
    std::cout << "Starting server..." << std::endl;
    int port_num = 9999;
    if (argc >= 2)
    {
        port_num = atoi (argv[1]);
    }
    std::cout << "Using port:         - [ " << port_num << " ]" << std::endl;
    if (port_num == 0)
    {
        std::cout << "Error: wrong port number" << std::endl;
        exit(1);
    }

    std::shared_ptr<ServerSocket> server = std::make_shared<ServerSocket> ();
    if (!server->Connect(port_num))
    {
        std::cout << "Error create server. Exit!" << std::endl;
        exit(1);
    }

    {//запуск потока на прослушку сокета и аксепт соединений. Так же на порождение потоков для обслуживания клиентов.
        std::thread t{main_thread, server};
        t.detach();
    }


    //В задании не сказано откуда именно сервер получает команды для фонарей. Я волюнтаристски реализовал получение команд с клавиатуры (консоль).
    ShowCaption();
    while (true)
    {
        std::string manual_cmd;
        std::cin >> manual_cmd;
        if ( manual_cmd == "1")
        {// послать всем фонарям команду 0x12
            std::shared_ptr <iCmd> cmd = std::make_shared<Cmd_On> ();
            CMD_SaveCmd (-1, cmd);
        } else if ( manual_cmd == "2")
        {// послать всем фонарям команду 0x13
            std::shared_ptr <iCmd> cmd = std::make_shared<Cmd_Off> ();
            CMD_SaveCmd (-1, cmd);
        } if ( manual_cmd == "4")
        {// послать одному фонарю команду 0x12
            std::shared_ptr <iCmd> cmd = std::make_shared<Cmd_On> ();
            int tmp_logic_id;
            std::cout << "Enter logic_id: ";
            std::cin >> tmp_logic_id;
            CMD_SaveCmd (tmp_logic_id, cmd);
        } else if ( manual_cmd == "5")
        {// послать одному фонарю команду 0x13
            std::shared_ptr <iCmd> cmd = std::make_shared<Cmd_Off> ();
            int tmp_logic_id;
            std::cout << "Enter logic_id: ";
            std::cin >> tmp_logic_id;
            CMD_SaveCmd (tmp_logic_id, cmd);
        } else if ( manual_cmd == "3" || manual_cmd == "6")
        {// послать всем фонарям команду 0x20
            int tmp_logic_id = -1;
            if (manual_cmd == "6")
            {
                std::cout << "Enter logic_id: ";
                std::cin >> tmp_logic_id;
            }
            auto tmp1 = std::make_shared<Cmd_SetColor> ();
            int r, g, b;
            std::cout << "Enter color (R G B) in hex (for example: 0x01 0x02 0xFF): ";
            
            std::cin >> manual_cmd;
            r = strtol(manual_cmd.c_str(), NULL, 16);
            std::cin >> manual_cmd;
            g = strtol(manual_cmd.c_str(), NULL, 16);
            std::cin >> manual_cmd;
            b = strtol(manual_cmd.c_str(), NULL, 16);
            tmp1->SetColor (r, g, b);
            std::shared_ptr <iCmd> cmd = tmp1;
            CMD_SaveCmd (tmp_logic_id, cmd);
        } else if ( manual_cmd == "9")
        {// убить клиентский поток
            std::cout << "Enter logic_id: ";
            int tmp_id;
            std::cin >> tmp_id;
            CMD_Flashligh_Kill(tmp_id);
        } else if ( manual_cmd == "0")
        {// убить все клиентские потоки
            CMD_Flashligh_Kill(-1);
        } else if ( manual_cmd == "h")
        {// 
            ShowCaption();
        }else if ( manual_cmd == "q")
        {// завершить работу
            return 0;
        }
    }
    return 0;
}





void client_thread(int logic_id, std::shared_ptr<ServerClnt> client_sock)
{
    auto my_stat_itr = _threads_stat.end(); //это будет итератор на структуру состояния данного потока (для ускорения обращения)
    for (auto itr = _threads_stat.begin(); itr != _threads_stat.end(); ++itr)
    {
        if ( itr->logic_id == logic_id)
        {
            my_stat_itr = itr;
            break;
        }
    }
    if (my_stat_itr == _threads_stat.end())
    {
        std::cout << "client_thread: some error (no state data)" << std::endl;
        return ;
    }

    CONTAINER_TYPE stream_buf;//создание контейнера для двоичного потока данных
    while (true)
    {//вечный цикл обработки
        std::this_thread::sleep_for(100ms); //чтобы не перегружать процессор. Если нужно ускорить реакцию отклика - можно попробовать заменить на std::this_thread::yield
        if (my_stat_itr->logic_id != logic_id)
        {
            std::cout << "client_thread: Very BIG TROUBLES" << std::endl; //произошло недопустимое - указатель на наше состояние содердит не наш ИД. Такого не должно быть. По идее надо тогда прибивать поток, но это уже обсуждается в каждом конкретном случае.
        }

        if (my_stat_itr->need_finish)
        {//нужно корректно завершить работу
            my_stat_itr->finished = true; // чтобы наш сборщик мусора удалил запись из пула процессов
            my_stat_itr->executed = false; // чтобы данному потоку больше не давали заданий
            std::cout << "client_thread: get command to die. Good bye!" << std::endl;
            return ;
        }

        if (my_stat_itr->executed == false)
        {//пришла команда на выполнение
            stream_buf.clear();
            //std::cout <<"DEBUG!!!! send type=" << (int)(my_stat_itr->data->GetType()) << std::endl;
            my_stat_itr->data->Pack(stream_buf);
            client_sock->SetData(stream_buf.data(), stream_buf.size());
            client_sock->Send();
            my_stat_itr->executed = true;
        }

    }
}

void create_client(int logic_id, std::shared_ptr<ServerClnt> client_sock)
{
    std::cout << "create_client: Create client: [logic_id = " << logic_id << " ]" << std::endl;
    ThreadStatus stat;
    stat.logic_id = logic_id;
    std::lock_guard<std::shared_mutex> l{_my_mutex}; //залочить доступ к списку
    _threads_stat.push_back(stat);
    std::thread t{client_thread, logic_id, client_sock}; //запустить поток для обслуживания клиента
    t.detach();
}

void remove_dead_clients_thread()
{//очистить список от закончивших работу потоков
    std::this_thread::sleep_for(1s); //проверяем раз в секунду
    std::lock_guard<std::shared_mutex> l{_my_mutex};
    for (auto itr = _threads_stat.begin(); itr != _threads_stat.end();)
    {
        if (itr->finished)
        {//поток сообщил о том, что он завершил свою работу. Удалить запись о нем.
            itr = _threads_stat.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}

void main_thread(std::shared_ptr<ServerSocket> server)
{
    int logic_id = 0;
    while(true)
    {
        int clnt_sock = server->Accept();
        if (clnt_sock < 0)
        {
            std::cout << "main_thread: Some error in connection with client" << std::endl;
            continue;
        }
        
        create_client (++logic_id,  std::make_shared <ServerClnt> (clnt_sock));
    }
}

void ShowCaption()
{
    std::cout << "Commands:" << std::endl;
    std::cout << "1 - switch On all flashlights" << std::endl;
    std::cout << "2 - switch Off all flashlights" << std::endl;
    std::cout << "3 - set color for all flashlights [R G B]" << std::endl;

    std::cout << "4 - switch On <id> flashlight" << std::endl;
    std::cout << "5 - switch Off <id> flashlight" << std::endl;
    std::cout << "6 - set color for <id> flashlight [R G B]" << std::endl;

    std::cout << "" << std::endl;
    std::cout << "9 - kill one flashlight <id>" << std::endl;
    std::cout << "0 - kill all flashlights" << std::endl;
    std::cout << "h - commands list" << std::endl;
    std::cout << "q - quit" << std::endl;
    std::cout << "" << std::endl;
}

void CMD_SaveCmd(int logic_id, std::shared_ptr<iCmd> cmd)
{
    std::lock_guard<std::shared_mutex> l{_my_mutex};
    for (auto &elem : _threads_stat)
    {
        if (logic_id < 0)
        {//даем команду всем готовым к вполнению команд фонарям
            if (elem.executed)
            {//даем команду только готовым к приему клиентам. Если требуется то можно изменить данную логику, добавив очередь команд.
                elem.cmd = cmd->GetType();
                elem.data = cmd;
                elem.executed = false;
            }
        }
        else
        {
            if (logic_id == elem.logic_id)
            {//даем команду только одному фонарю
                if (elem.executed)
                {//даем команду только готовым к приему клиентам. Если требуется то можно изменить данную логику, добавив очередь команд.
                    elem.cmd = cmd->GetType();
                    elem.data = cmd;
                    elem.executed = false;
                }
                break;
            }
        }
    }
}

void CMD_Flashligh_Kill(int logic_id)
{
    std::lock_guard<std::shared_mutex> l{_my_mutex};
    for (auto &elem : _threads_stat)
    {
        if (logic_id < 0)
        {//даем команду всем фонарям
            elem.need_finish = true;
        }
        else
        {
            if (logic_id == elem.logic_id)
            {//даем команду только одному фонарю
                elem.need_finish = true;
                break;
            }
        }
    }
}
