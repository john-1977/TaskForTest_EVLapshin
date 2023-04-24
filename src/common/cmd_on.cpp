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
	return 3 + _len; //������� ������� ���� ���������
}

CONTAINER_TYPE::iterator Cmd_On::Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start)
{
	start = UnpackHeader(buf, start);
	
	//��� ��� ������ � ���� ������� ��� �� start ��� ��������� �� ������ ����� �������
	return start;
}

bool Cmd_On::execute()
{//��� ��� �������� ������ �� ������������ �� ������  ��������� ��������� 
	std::cout << "Executing cmd: ON (0x12)." << std::endl;
	return true;
}
