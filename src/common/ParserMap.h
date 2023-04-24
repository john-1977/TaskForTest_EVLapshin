#pragma once
#include <iostream>
#include "iCmd.h"
#include <map>
#include <memory>


///Контейнер для хранения соответствия "Тип <---> парсер". Здесь я закладывался на тот случай, когда один тип команды разбирает только один парсер. Если требуется сделать цепочку обработчиков - то надо переделать.
class ParserMap
{
public:
	ParserMap();

	template <typename T> bool AddParser(); //добавление разборщика в контейнер
	std::shared_ptr<iCmd> GetParser(BYTE type);//получить для указанного типа парсер в виде std::shared_ptr<iCmd>.
	template <typename T> T* GetParserPtr();//получить указатель на реальный тип парсера. Указатель после использования не удалять - он используется в shared_ptr (TODO это потенциально узкое и опасное место, продумать как можно поменять)
	bool IsParserExist(BYTE type); //определяет по типу команды, есть ли такой парсер в контейнере

protected:
	std::map<BYTE, std::shared_ptr<iCmd> > _map;
};






//имплементация шаблонов

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
	{//запрошенного парсера не существует - кинуть исключение
		std::string err = "Can't find parser with type = " + std::to_string(type);
		std::cout << err << std::endl;
		throw std::logic_error(err);
	}

	return dynamic_cast<T*> (itr->second.get());
}

