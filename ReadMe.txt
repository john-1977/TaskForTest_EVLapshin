Чтобы скомпилировать проект:
1. выполнить скрипт ./MakeSln.sh
 В результате должна появиться папка bld с проектом.
2. Выполнить команду cd bld
3. make -j 4
 Должны появиться файлы bld/src/client/my_client и bld/src/server/my_server которые и являются выполнимыми файлами проекта.
 Можно запускать без ключей (порт по умолчанию 9999).

 Для того, чтобы можно было подавать команды в сервере реализовано взаимодействие с пользователем посредством ввода команд в консоли.
 Команды:
 1 - послать всем подключившимся по сети фонарям команду на включение (0х12).
 2 - послать всем подключившимся по сети фонарям команду на выключение (0х13).
 3 - послать всем подключившимся по сети фонарям команду на изменение цвета (0х20). При этом надо будет дополнительно
 ввести код цвета (3 шестнадцатеричных числа через пробел).
 4,5,6 - аналогично 1,2,3 но команда подается только одному клиенту. Клиент выбирается заданием logic_id.
 9 - убить один поток отвечающий за взаимодействие с клиентом. Выбор осуществляется по logic_id.
 0 - убить все имеющиеся потоки (но не закрывать сервер, тоесть новые клиенты могут подключаться).
 h - вывести списко команд на экран
 q - завершить работу сервера


Общие пояснения по программе.
    Основной упор делался на то, чтобы можно было легко задавать новые команды. Все они наследуют чисто виртуальный интерфейсный класс iCmd.
Метод Pack работает на стороне сервера, упаковывая данные для отправки. Метод Unpack работает на стороне клиента, распаковывая
поток байтов во внутренние переменные класса. Метод execute выполняется после успешного разбора команды (симулирует выполняемую программой
работу). Создавая аналогичные классы-наследники можно реализовать практически любую команду.
    Разбор приходящих пакетов делается при помощи потокового парсера, который использует реализованные классы команд. Для того, чтобы
команда использовалась, ее нужно сначала добавить в парсер. Если приходит команда, для которой нет реализованного парсера, то используется
разборщик по умолчанию (просто пропускается данная команда).
    Сервер сделан многопоточным. На каждого клиента заводится по одному потоку. Так же есть отдельный поток сборщика мусора, поток
отслеживающий подключения клиентов и основной поток принимающий команды пользователя с клавиатуры. Каждому новому клиенту (и потоку)
задается его уникальный идентификатор (logic_id). Формирование данного идентификатора происходит простым инкрементированием. Адресные
команды, обращающиеся к определенным потокам, используют именно данный идентификатор.
    Так как это тестовое задание, то иногда для одних и тех же задач использовал разные решения (просто чтобы продемонстрировать знания).
Хотел сделать документацию на Doxygen-e но времени на программу уже много потратил, поэтому не стал этим заморачиваться. Если будет нужно - сделаю.

    Программа тестировалась на ОС Ubuntu 22.10. Все что требуется - работает (ну на сколько я это вижу).
