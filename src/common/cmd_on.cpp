#include "cmd_on.h"
#include <iostream>

Cmd_On::Cmd_On()
{
	_type = 0x12;
}

int Cmd_On::Pack(CONTAINER_TYPE &buf)
{
	_len = 0;
	PackHeader(buf);
	return 3 + _len; //вернуть сколько было добавлено
}

CONTAINER_TYPE::iterator Cmd_On::Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start)
{
	start = UnpackHeader(buf, start);
	
	//так как данных в этой функции нет то start уже указывает на начало новой команды
	return start;
}

bool Cmd_On::execute()
{//“ак как реальной работы не производитс€ то просто  выводитс€ сообщение 
	std::cout << "Executing cmd: ON (0x12)." << std::endl;
	return true;
}
