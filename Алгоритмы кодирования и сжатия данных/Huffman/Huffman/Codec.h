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
	/// ������� � ����� ���
	///</summary>
	///<param name="os">����� ������</param>
	///<param name="�">��� ��� ������ � �����</param>
	static void outputCode(ofstream &os, const HC::Code &c);

	///<summary>
	/// ������ ��� �� ������
	/// ������������� ���������� outputCode
	///</summary>
	///<param name="is">����� �����</param>
	///<param name="bit">����������� ���</param>
	///<returns>true, ���� � ������ ���� ������, ����� - false</returns>
	static bool getBit(ifstream &is, int &bit);

public:
	
	static int CompressFile(ifstream &is, ofstream &os);
	static int DeCompressFile(ifstream &is, ofstream &os);

#ifdef WITH_BLOCKS
	enum B_TYPE {Huffman, RLE};
	/// <summary>
	/// ��� ��� ��������� ����� ����� ��������
	/// </summary>
	/// <param name="len">����� �����</param>
	/// <returns>��������� ����� ����� ������</returns>
	static HC::Code MakeCode(int len) {
		if (len > MAX_BLOCK_LEN)
			len = len + 1;
		assert(len>=0); 
		assert(len <= MAX_BLOCK_LEN);
		return HC::Code(BLOCK_LEN_BITS+1, len);
	}
	/// <summary>
	/// ��� ��� ��������� � ����� RLE 
	/// </summary>
	/// <param name="len">���������� ��������</param>
	/// <param name="ch">������������� ������</param>
	/// <returns>��� RLE c ��������� RLE � ������������� ��������</returns>
	static HC::Code MakeCode(int len, unsigned char ch) {
		return HC::Code(BLOCK_LEN_BITS + 9, ((1<< BLOCK_LEN_BITS) + len<<8) + ch);
	}

	/// <summary>
	/// ������ �� �������� ������ ����� ����� � ��������� �����
	/// </summary>
	/// <param name="is">������� �����</param>
	/// <returns>����� �����</returns>
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
	/// ������� ������ ������� � �����, � ������� ���������� ������� �������.
	/// ���� ��������� ���� �� �������� ������ ��������, �� � ����� ������ ����������� ���� 
	/// (pos, 0), ��� pos - ����� �����, � 0 ������� ����������� ������.
	/// </summary>
	/// <param name="is">������� �����</param>
	/// <param name="n">����������� ���������� ��������</param>
	/// <returns>������ (�������, �����) �������* ��. ����</returns>
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
				if (len == 0)	// ������ �������� �������
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

