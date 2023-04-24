#pragma once
#include <vector>

class iCmd
{
    public:
    iCmd(){};

    /** \brief Метод упаковки данных
      
    Преобразует данные из внутреннего формата в двоичный поток данных, пригодных для отправки.
    [\in] itr - итератор, указывающий на начальную позицию в выходном буфере 
    [\return] количество упакованных байт (включая заголовок). В случае ошибки возвращает 0.
    */
    virtual int PackData(std::vector<unsigned char>::iterator itr) = 0;

    /** \brief Метод распаковки данных
      
      Преобразует данные из двоичного потока во внутренний формат данных.
     [\in] itr - итератор, указывающий на начальную позицию в принятом буфере двоичных данных.
     [\return] количество упакованных байт (включая заголовок). В случае ошибки возвращает 0.
    */
    virtual int UnpackData(std::vector<unsigned char>::iterator itr) = 0;

};