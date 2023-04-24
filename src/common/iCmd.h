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
	virtual int Pack(CONTAINER_TYPE &buf) = 0; //��������� ������ � ����� ������. ���������� ����������� ���������� ����.
	virtual CONTAINER_TYPE::iterator Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start) = 0; //��������� ���������� ��������� � ������� ������� � ��������� �������. ������� �������� �� ������ ��������� �������.
	virtual bool execute() { return true; };//������ ������� ������ ����������� ��� ��������� ������� (����� �� �������). ������������ �������� ����� ������������� �� ������� � �������.

protected:
	void PackHeader(CONTAINER_TYPE &buf, size_t start_packet = -1) //��������� ��������� � �������� �����. ����� ������������ �� ���������� _len.
	{
		if (start_packet == -1)
		{//�������� � ����� ������
			buf.push_back(_type);
			buf.push_back((_len & 0xff00) >> 8); //������� ����
			buf.push_back(_len & 0xff); //������� ����
		}
		else
		{//����� � ���������� �������
			buf[start_packet++] = _type; //TODO ������������� ������� ����� - ���� ������ ����� ������������ ��� ������ �� ���������� �������. ��� ������������� - �������� ��������.
			buf[start_packet++] = (_len & 0xff00) >> 8; //������� ����
			buf[start_packet++] = _len & 0xff; //������� ����
		}
	}

	CONTAINER_TYPE::iterator UnpackHeader(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start)
	{
		_type = *start; ++start;
		_len = *start; ++start;
		_len = _len << 8; //������� ����
		_len += *start; ++start;//������� ����
		return start;
	}

protected:
	BYTE _type;
	unsigned short _len;

};