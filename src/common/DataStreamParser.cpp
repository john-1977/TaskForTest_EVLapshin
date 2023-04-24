#include "DataStreamParser.h"

DataStreamParser::DataStreamParser(std::shared_ptr<ParserMap> parser_map)
{
	_parser_map = parser_map;
}

bool DataStreamParser::ParseBuffer(CONTAINER_TYPE &buf)
{
	CONTAINER_TYPE::iterator itr = buf.begin();

	try
	{
		while (itr != buf.end())
		{
			BYTE type = *itr;
			//std::cout << "DEBUG: type = " << (int)type << std::endl;
			if (_parser_map->IsParserExist(type))
			{//���� ����������� ������ ��� ������ �������
				auto concrete_parser = _parser_map->GetParser(type);
				itr = concrete_parser->Unpack(buf, itr);
				concrete_parser->execute();
			}
			else
			{//������������ ������� ��� - �������� ������������ ���������
				++itr;//��� ������� ��� ���� ����
				std::cout << "Using default parser for type: " << (int)type << std::endl;
				
				unsigned short len = (*itr) << 8; //����� ������
				++itr;
				len += *itr;
				++itr;

				//������ itr ��������� �� ������ ������. ���� �� ���������� ����� ������� � ��������� �������. �� ������� ������� �������� �� ����� �� ������� �������
				auto a = itr - buf.begin() + len;
				if (itr - buf.begin() + len > buf.size())
				{
					throw std::logic_error(std::string("Bad parse"));
				}
				itr += len;
				
			}
		}
		std::cout << "Good parse." << std::endl;
		return true;
	}
	catch (std::logic_error &e)
	{
		std::cout << "Logic error: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ERROR! Unknown exception." << std::endl;
	}
	return false;
}
