#pragma once
#include "iCmd.h"
class Cmd_SetColor : public iCmd
{
public:
	Cmd_SetColor();

	int Pack(CONTAINER_TYPE &buf);
	CONTAINER_TYPE::iterator Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start);
	bool execute();
	void SetColor(const BYTE r, const BYTE g, const BYTE b); //установить цвет
	void GetColor (BYTE &r, BYTE &g, BYTE &b); //получить текущий цвет
protected:
#pragma pack (push)
#pragma pack (1)
	struct
	{
		BYTE r, g, b;
	} _color;
#pragma pack (pop)
};

