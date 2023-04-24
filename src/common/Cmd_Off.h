#pragma once
#include "iCmd.h"
class Cmd_Off :	public iCmd
{
public:
	Cmd_Off();
	int Pack(CONTAINER_TYPE &buf);
	CONTAINER_TYPE::iterator Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start);
	bool execute();
protected:
};

