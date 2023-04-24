#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <iCmd.h>
#include <ParserMap.h>
#include <Cmd_Off.h>
#include <cmd_on.h>
#include <Cmd_SetColor.h>
#include <DataStreamParser.h>
#include <MySocket.h>

#define DBG 1

int main (int argc, char* argv[])
{
    std::cout << "Hello from client!" << std::endl;
    ClientSocket sock_client;
    {
        int port_num {9999};
        std::string addr {"127.0.0.1"};//дефолтные значения
        if (argc >1)
        {//первым аргументом должен передаваться адрес
            addr = argv[1];
        }

        if (argc >2)
        {//вторым параметром передается порт
            port_num = atoi(argv[2]);
            if (port_num == 0)
            {
                std::cout << "Error port number." << std::endl;
                exit(1);
            }
        }

        if (!sock_client.Connect(addr, port_num))
        {
            std::cout << "Some error in socket" << std::endl;
            exit(1);
        }
    }
    
    std::shared_ptr<ParserMap> pars_map = std::make_shared<ParserMap> ();
	//добавляем те команды, поддежку которых мы хотим реально иметь (не обязательно добавлять все реализованные парсеры)
	pars_map->AddParser<Cmd_On>();
	pars_map->AddParser<Cmd_Off>();
	pars_map->AddParser<Cmd_SetColor>();

    CONTAINER_TYPE stream_buf;//создание контейнера для двоичного потока данных
    DataStreamParser dsp(pars_map);

    while (true)
    {//вечный цикл обработки команд
        unsigned char buf[1024];
        int recv_len = sock_client.Recv(buf, sizeof(buf));        

        if (recv_len == 0)
        {
            std::cout << "Connection is broken. Exit." <<std::endl;
            break;
        }
        if (recv_len > 0)
        {//разобрать полученный буффер
            stream_buf.assign(buf, &buf[recv_len]);

            #ifdef DBG
            {//for debug only
                std::cout << "Buffer: " << std::hex;
                std::for_each(std::begin(stream_buf), std::end(stream_buf), 
                    [](const auto &elem)
                {
                    std::cout << (int)elem << ":";
                }
                );
                std::cout << std::endl;
            }//end debug
            #endif

            dsp.ParseBuffer(stream_buf);


        }
    }
    
    return 0;
}