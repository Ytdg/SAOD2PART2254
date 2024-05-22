#pragma once
#include "HC.h"
#include <list>
#include <cassert>

//#define WITH_BLOCKS

#ifdef WITH_BLOCKS
#define BLOCK_LEN_BITS 13
#define MAX_BLOCK_LEN	((1<<BLOCK_LEN_BITS))
#endif


class Codec
{
	///<summary>
	/// Выводит в поток код
	///</summary>
	///<param name="os">поток вывода</param>
	///<param name="с">код для вывода в поток</param>
	static void outputCode(ofstream &os, const HC::Code &c);

	///<summary>
	/// Читает бит из потока
	/// Соответствуте реализации outputCode
	///</summary>
	///<param name="is">поток ввода</param>
	///<param name="bit">прочитанный бит</param>
	///<returns>true, если в потоке есть данные, иначе - false</returns>
	static bool getBit(ifstream &is, int &bit);

public:
	
	static int CompressFile(ifstream &is, ofstream &os);
	static int DeCompressFile(ifstream &is, ofstream &os);

#ifdef WITH_BLOCKS
	enum B_TYPE {Huffman, RLE};
	/// <summary>
	/// Код для заголовка блока кодов Хаффмана
	/// </summary>
	/// <param name="len">длина блока</param>
	/// <returns>Заголовок блока кодов длиной</returns>
	static HC::Code MakeCode(int len) {
		if (len > MAX_BLOCK_LEN)
			len = len + 1;
		assert(len>=0); 
		assert(len <= MAX_BLOCK_LEN);
		return HC::Code(BLOCK_LEN_BITS+1, len);
	}
	/// <summary>
	/// Код для заголовка и блока RLE 
	/// </summary>
	/// <param name="len">количество повторов</param>
	/// <param name="ch">повторяющийся символ</param>
	/// <returns>Код RLE c признаком RLE и повторяющимся символом</returns>
	static HC::Code MakeCode(int len, unsigned char ch) {
		return HC::Code(BLOCK_LEN_BITS + 9, ((1<< BLOCK_LEN_BITS) + len<<8) + ch);
	}

	/// <summary>
	/// Читает из входного потока длину блока в заголовке блока
	/// </summary>
	/// <param name="is">входной поток</param>
	/// <returns>длину блока</returns>
	static int getBlockLen(ifstream& is)
	{
		int blockLen = 0, bit;
		for (int i = 0; i < BLOCK_LEN_BITS; i++) {
			getBit(is, bit);
			blockLen = (blockLen << 1) + bit;
		}
		return blockLen;
	}

	/// <summary>
	/// Находит список позиций в файле, в которых начинаются повторы символа.
	/// Если последний блок не является блоком повторов, то в конец списка добавляется пара 
	/// (pos, 0), где pos - длина файла, а 0 признак специальной записи.
	/// </summary>
	/// <param name="is">входной поток</param>
	/// <param name="n">минимальное количество повторов</param>
	/// <returns>список (позиция, длина) повтров* см. выше</returns>
	static std::list<pair<int, int>> Analyze(ifstream& is, int n = 10)
	{
		std::list<pair<int, int>> res;

		char chCurrent, chPrev;
		is.get((char&)chPrev);
		int begin = 0, len = 0;
		int pos = 0;
		while (!is.eof())
		{
			is.get((char&)chCurrent);
			if (chPrev == chCurrent)
			{
				if (len == 0)	// Начало повторов символа
					begin = pos;
				len++;
			}
			else
			{
				if (len > n) 
					res.push_back(make_pair(begin, len+1));
				len = 0;
				chPrev = chCurrent;
			}
			pos++;
		}
		if (chCurrent != chPrev || len < n)
			res.push_back(make_pair(pos, 0));
		else
			res.push_back(make_pair(begin, len));

		return res;
	}
#endif
};

