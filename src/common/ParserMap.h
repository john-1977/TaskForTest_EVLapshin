#pragma once
#include <iostream>
#include "iCmd.h"
#include <map>
#include <memory>


///��������� ��� �������� ������������ "��� <---> ������". ����� � ������������ �� ��� ������, ����� ���� ��� ������� ��������� ������ ���� ������. ���� ��������� ������� ������� ������������ - �� ���� ����������.
class ParserMap
{
public:
	ParserMap();

	template <typename T> bool AddParser(); //���������� ���������� � ���������
	std::shared_ptr<iCmd> GetParser(BYTE type);//�������� ��� ���������� ���� ������ � ���� std::shared_ptr<iCmd>.
	template <typename T> T* GetParserPtr();//�������� ��������� �� �������� ��� �������. ��������� ����� ������������� �� ������� - �� ������������ � shared_ptr (TODO ��� ������������ ����� � ������� �����, ��������� ��� ����� ��������)
	bool IsParserExist(BYTE type); //���������� �� ���� �������, ���� �� ����� ������ � ����������

protected:
	std::map<BYTE, std::shared_ptr<iCmd> > _map;
};






//������������� ��������

template <typename T>
bool ParserMap::AddParser()
{
	std::shared_ptr<iCmd> concrete_parser = std::make_shared<T>();
	BYTE type = concrete_parser->GetType();
	if (_map.count(type))
	{
		std::cout << "Warning: parser's duplicate is not allowed." << std::endl;
		return false;
	}

	_map[type] = concrete_parser;
	return true;
}

template <typename T>
T* ParserMap::GetParserPtr()
{
	BYTE type = T().GetType();
	auto itr = _map.find(type);
	if (_map.end() == itr)
	{//������������ ������� �� ���������� - ������ ����������
		std::string err = "Can't find parser with type = " + std::to_string(type);
		std::cout << err << std::endl;
		throw std::logic_error(err);
	}

	return dynamic_cast<T*> (itr->second.get());
}

