#include "ParserMap.h"
#include  <string>

ParserMap::ParserMap()
{

}

std::shared_ptr<iCmd> ParserMap::GetParser(BYTE type)
{
	auto itr = _map.find(type);
	if (_map.end() == itr)
	{//запрошенного парсера не существует - кинуть исключение
		std::string err = "Can't find parser with type = " + std::to_string(type);
		std::cout << err << std::endl;
		throw std::logic_error(err);
	}
	return itr->second;
}

bool ParserMap::IsParserExist(BYTE type)
{
	return _map.count(type);
}
