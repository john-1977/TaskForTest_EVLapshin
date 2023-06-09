#pragma once
#include "iCmd.h"
#include "ParserMap.h"
#include <memory>

///Класс разбирающий поток двоичных данных с командами
class DataStreamParser
{
protected:
	DataStreamParser() = default;

public:
	
	DataStreamParser(std::shared_ptr<ParserMap> parser_map);

	//void SetParserMap(std::shared_ptr<ParserMap> parser_map);
	bool ParseBuffer(CONTAINER_TYPE &buf); //полностью разобрать переданный буффер. В случае успеха возвращается true, иначе false

protected:
	std::shared_ptr<ParserMap> _parser_map;
};

