// Parsing.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <algorithm>

#include "iCmd.h"
#include "ParserMap.h"
#include "Cmd_Off.h"
#include "cmd_on.h"
#include "Cmd_SetColor.h"

#include "DataStreamParser.h"


void Test1()
{//проверка парсинга
	std::shared_ptr<ParserMap> pars_map = std::make_shared<ParserMap> ();

	//добавляем те команды, поддежку которых мы хотим реально иметь (не обязательно добавлять все реализованные парсеры)
	pars_map->AddParser<Cmd_On>();
	pars_map->AddParser<Cmd_Off>();
	pars_map->AddParser<Cmd_SetColor>();
	

	CONTAINER_TYPE buf;//создание контейнера для двоичного потока данных

	//заполнение буфера командами
	{
		{
			Cmd_On cmd;
			cmd.Pack(buf);
		}
		{
			Cmd_Off cmd;
			cmd.Pack(buf);
		}
		{
			Cmd_SetColor cmd;
			cmd.SetColor(1, 2, 3);
			cmd.Pack(buf);
		}
		{
			Cmd_Off cmd;
			cmd.Pack(buf);
		}

		std::cout << "Buffer: " << std::hex;
		std::for_each(std::begin(buf), std::end(buf), 
			[](const auto &elem)
		{
			std::cout << (int)elem << ":";
		}
		);
			std::cout << std::endl;
	}

	//разбор потока команд
	{
		DataStreamParser dsp(pars_map);

		dsp.ParseBuffer(buf);
	}

}
