#include "Cmd_Off.h"
#include <iostream>

Cmd_Off::Cmd_Off()
{
	_type = 0x13;
}

int Cmd_Off::Pack(CONTAINER_TYPE &buf)
{
	_len = 0;
	PackHeader(buf);
	return 3 + _len; //вернуть сколько было добавлено
}

CONTAINER_TYPE::iterator Cmd_Off::Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start)
{
	start = UnpackHeader(buf, start);

	//так как данных в этой функции нет то start уже указывает на начало новой команды
	return start;
}

bool Cmd_Off::execute()
{//Так как реальной работы не производится то просто  выводится сообщение 
	std::cout << "Executing cmd: OFF (0x13)." << std::endl;
	return true;
}

