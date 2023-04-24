#pragma once
#include "iCmd.h"
class Cmd_On : public iCmd
{
public:
	Cmd_On();
	
	int Pack(CONTAINER_TYPE &buf);
	CONTAINER_TYPE::iterator Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start);
	bool execute();

protected:

};

