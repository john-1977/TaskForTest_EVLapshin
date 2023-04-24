#pragma once

#include <vector>
#include <string>

typedef unsigned char BYTE;
typedef std::vector<BYTE> CONTAINER_TYPE;

class iCmd
{
public:
	iCmd() : _type(0), _len(0) {}
	virtual BYTE GetType() { return _type; }
	virtual unsigned short GetLen() { return _len; }
	virtual int Pack(CONTAINER_TYPE &buf) = 0; //добавл¤ет данные в конец буфера. ¬озвращает добавленное количество байт.
	virtual CONTAINER_TYPE::iterator Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start) = 0; //–азобрать переданный контейнер с данными начина¤ с указанной позиции. ¬ернуть итератор на начало следующей команды.
	virtual bool execute() { return true; };//работа котора¤ должна выполн¤тьс¤ при получении команды (после ее разбора). ¬озвращаемое значение может варьироватьс¤ от команды к команде.

protected:
	void PackHeader(CONTAINER_TYPE &buf, size_t start_packet = -1) //”паковать заголовок в двоичный буфер. ƒлину используетс¤ из переменной _len.
	{
		if (start_packet == -1)
		{//дописать в конец буфера
			buf.push_back(_type);
			buf.push_back((_len & 0xff00) >> 8); //старший байт
			buf.push_back(_len & 0xff); //младший байт
		}
		else
		{//пишем с указанного индекса
			buf[start_packet++] = _type; //TODO потеенциально опасное место - если буфера будет недостаточно дл¤ записи то произойдет падение. ѕри необходимости - добавить проверку.
			buf[start_packet++] = (_len & 0xff00) >> 8; //старший байт
			buf[start_packet++] = _len & 0xff; //младший байт
		}
	}

	CONTAINER_TYPE::iterator UnpackHeader(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start)
	{
		_type = *start; ++start;
		_len = *start; ++start;
		_len = _len << 8; //старший байт
		_len += *start; ++start;//младший байт
		return start;
	}

protected:
	BYTE _type;
	unsigned short _len;

};