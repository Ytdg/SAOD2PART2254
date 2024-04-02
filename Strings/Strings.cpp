// Strings.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#define  _CRT_SECURE_NO_WARNINGS
#include "Str.h"
void Test(Str s){}
int main()
{
    Str *test1 =new Str("bbb");
    Str* test2 = new Str("ccc");
    *test2 += *test1;
    std::cout << *test1+*test2 << std::endl;

    


}

