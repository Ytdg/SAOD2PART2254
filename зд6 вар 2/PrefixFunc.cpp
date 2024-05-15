#include <iostream>
#include "PF.h"
using namespace std;

int main()
{
	// Инициализации объекта PF строкой "rakaratrakarakarakatakarakara"
	PF prefixFunc = "rakaratrakarakarakatakarakara";

	for (int i = 0; i < prefixFunc; i++)
	{
		cout << prefixFunc[i];
	}
	cout << endl;
	// Результат верный - "00001201234563456340000123456"

	// Вывод количества сравнений для "rakaratrakarakarakatakarakara"
	cout << prefixFunc.CmpCount() << endl;

	// Цикл для генерации 5 случайных строк длиной в 100 символов
	for (int i = 0; i < 5; i++) {
		const int N = 100; // Длина генерируемой строки
		char str[N]{ 0 }; // Массив для хранения строки с нулевым символом в конце

		// Генерация случайной строки из символов 'a', 'b', 'c', 'd'
		for (int j = 0; j < N - 1; j++) {
			str[j] = rand() % ('d' - 'a' + 1) + 'a'; // Случайный символ от 'a' до 'd'
		}

		// Вывод сгенерированной строки
		for (char* p = str; *p; cout << *(p++));
		cout << " " << strlen(str) << endl; // Вывод длины строки

		// Создание объекта префикс-функции для сгенерированной строки
		PF prefixFunc = str;

		// Вывод количества сравнений символов, выполненных при вычислении префикс-функции
		cout << prefixFunc.CmpCount() << endl;
	}
}
	
