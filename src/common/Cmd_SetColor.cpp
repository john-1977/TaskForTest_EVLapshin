#include "Cmd_SetColor.h"
#include <algorithm>
#include <sstream>
#include <iostream>

Cmd_SetColor::Cmd_SetColor()
{
	_type = 0x21;
}

int Cmd_SetColor::Pack(CONTAINER_TYPE &buf)
{
	size_t start_packet = buf.size(); //сохраним индекс начала пакетав буфере. Этонам пригодится на случай, если мы не можем заранее предсказать длину данных
	_len = 0;
	PackHeader(buf);//добавим заголовок (пока длина данных указана неправильная, так как здесь я хочу показать как работать с данными, длину которых мы заранее не знаем)

	//добавление красного цвета
	buf.push_back(_color.r);
	_len += sizeof(_color.r);

	//добавление зеленого цвета
	buf.push_back(_color.g);
	_len += sizeof(_color.g);

	//добавление синего цвета
	buf.push_back(_color.b);
	_len += sizeof(_color.b);
	
	//теперь в переменной _len находится правильная длина добавленных данных - требуется обновить заголовок пакета
	PackHeader(buf, start_packet);
	return 3 + _len;

}

CONTAINER_TYPE::iterator Cmd_SetColor::Unpack(CONTAINER_TYPE &buf, CONTAINER_TYPE::iterator start)
{
	start = UnpackHeader(buf, start);


	int cnt = sizeof(_color);
	if (_len != cnt)//здесь проверка на ожидаемую длину данных и реально полученную. Длина в посылке может быть испорчена либо случайно, либо преднамеренно (злоумышленниками). Подобную проверку надо проводить в каждой команде, но так как это тестовое задание - я не буду утомлять таким количеством однообразного кода.
	{
		std::stringstream ss;
		ss << "Protocol error: [type=" << _type << ", expected length: " << cnt << ", real length: " << _len << " ]";
		throw std::logic_error(ss.str());
	}

	//так как это тестовое задание то разберу посылку несколько другим способом - просто чтобы показать знание разных техник. В реальной программе лучше делать все единообразно, чтобы потом легче было сопровождать код.
	for (BYTE* tmp_ptr = (BYTE*)static_cast<void*> (&_color); cnt; --cnt, ++start, ++tmp_ptr)
	{
		*(static_cast<BYTE*> (tmp_ptr)) = *start;
	}

	return start;
}

bool Cmd_SetColor::execute()
{
	std::cout << "Executing cmd: SetColor (0x20). Color is (R,G,B): " << (int)_color.r << ", " << (int)_color.g << ", " << (int)_color.b << std::endl;
	return true;
}

void Cmd_SetColor::SetColor(const BYTE r, const BYTE g, const BYTE b)
{
	_color.r = r;
	_color.g = g;
	_color.b = b;
}

void Cmd_SetColor::GetColor(BYTE &r, BYTE &g, BYTE &b)
{
	r = _color.r;
	g = _color.g;
	b = _color.b;
}
