#include "stdafx.h"
#include "Codec.h"


void Codec::outputCode(ofstream &os, const HC::Code &c)
{
	static unsigned char cBuffer;		// ��� �������� �������� ����������� ����
	static int iNumberOfBitsInBuffer;	// ��� �������� ����� ������� � �����

	// ���������� �����������
	if (c.len > 0) {
		int iCode = c.bits;
		unsigned int selector = 1 << (c.len - 1);
		while(selector != 0)
		{
			// �������� � ����� ���� �������� ����
			cBuffer <<= 1;
			cBuffer += (selector&iCode) != 0? 1:0;
			selector >>= 1;
			iNumberOfBitsInBuffer++;
			// ���� �������� ����, �� ����� ��� � �������� �����, ���������� ����� � �������� ����������� ��������� ����
			if (iNumberOfBitsInBuffer == 8) {
				os.put(cBuffer);
				cBuffer = (unsigned char)0;
				iNumberOfBitsInBuffer = 0;
			}
		}
	}
	// ����� � ����� ��������� ��� (������� �����) � ��������� ���� - ���������� ����� � ��������� �����
	else 
	{
		if (iNumberOfBitsInBuffer != 0)
		{
			os.put(cBuffer << (8 - iNumberOfBitsInBuffer));
			os.put((char)(iNumberOfBitsInBuffer));
		}
		else
			os.put(8);
	}
}


bool Codec::getBit(ifstream &is, int &bit)
{
	static bool init = true;
	static char last;
	static char buff;
	static unsigned char mask;

 	if (init)
	{
		init = false;
		is.get(last);
	}
	// ������ ��������� ����
	if (mask == 0 && !is.eof())
	{
		buff = last;
		is.get(last);
		mask = 0x80;	// ������� ��� �����
	}
	
	// ��������� ���� ��� �������. � ��� ���������� �������� ����� � ���������� �����
	if (is.peek() == EOF)
		if (last > 0) // �������� �������� ����
		{
			bit = (mask&buff) != 0 ? 1 : 0;
			mask >>= 1;				// �������� � ���������� ����
			last--;
			return true;
		}
		else
			return false;

	bit = (mask&buff) != 0 ? 1 : 0;
	mask >>= 1;				// �������� � ���������� ����
	return true;
}

int Codec::CompressFile(ifstream& is, ofstream& os)
{
	map<char, int> chTbl;
	unsigned char chCurrent;
	is.get((char&)chCurrent);
	while (!is.eof())
	{
		chTbl[chCurrent]++;
		is.get((char&)chCurrent);
	}

	/// ���������� ����� ��������
	HC hc;
	hc.GenerateCodes(chTbl);

	/// ���������� ������ � ����� ������� �����
	int n = 0;
	for (int i = 0; i < hc.TblSize(); i++)
		if (hc.GetCode(i).len)
			n++;

	// ����� ������� ����� � ����
	// int n - ���������� ������� � �������
	// {ch, len, code} �� ����� �������
	os.write((char*)&n, sizeof(n));
	for (int i = 0; i < hc.TblSize(); i++) {
		HC::Code c = hc.GetCode(i);
		if (c.len != 0)
		{
			os.put(c.ch);
			os.put(c.len);
			os.write((char*)&c.bits, sizeof(c.bits));
		}
	}

	/// ���������� ������ �������� ������ � ��� �����������
	is.clear();
	is.seekg(0, ios::beg);

#ifdef WITH_BLOCKS
	auto r = Analyze(is);

	is.clear();
	is.seekg(0, ios::beg);

	int blockLen = 0;
	int nBlk = 0;

	auto rit = r.begin();		// �������� ��� ������ ��������
	int pos = 0, bbPos=0;		// ������� � ����� � ������� ������ �����
	is.get((char&)chCurrent);
	while (!is.eof())
	{
		if (rit != r.end() && rit->second > 0)
		{
			if (pos < rit->first &&  bbPos < MAX_BLOCK_LEN)	//�������� �� ��������
			{
				if(bbPos++ == 0)	// ������� ��������� ����� ����� �� �������� � ������� ����� �����
					outputCode(os, MakeCode(rit->first - pos));

				outputCode(os, hc.GetCode(chCurrent));
			}
			else if (pos == rit->first) // ������� ��������� ��� RLE
			{
				outputCode(os, MakeCode(rit->second, chCurrent));
			}
			if (pos + 1 == rit->first + rit->second) {		// �������� ���� ����� ��������
				rit++;										// ������ �������� ���������� �������.
				bbPos = 0;									// ���������� ������ ����� �� ��������.
			}
		}
		else {					// ������ ������ RLE ���
			if (bbPos++ == 0 && rit != r.end()) {			// ������� ��������� ����� ����� �� ��������
				outputCode(os, MakeCode(rit->first - pos));	// ����� ����� ������� ����� �� �����!
			}
			outputCode(os, hc.GetCode(chCurrent));
		}

		is.get((char&)chCurrent);
		pos++;
	}
	outputCode(os, HC::Code(0, 0));
#else
	is.get((char&)chCurrent);
	while (!is.eof())
	{
		outputCode(os, hc.GetCode((unsigned char)chCurrent));
		is.get((char&)chCurrent);
	}
	outputCode(os, HC::Code(0, 0));
#endif
	return 0;
}

/*
���� ���������� ������� DeCompressFile:

1. ��������� �� �������� ������ ���������� ����� ��������.
2. ������� ������ HC ��� �������� ����� ��������.
3. ��������� � ��������� ���� �������� �� �������� ������:
   a. ��������� ������, ����� ���� � ���� ���� ����.
   b. ��������� ��������� ������ � ������� HC.
4. ������������ ������ �������� �� ���������� �����.
5. ��������������� ������ ���� �� �������� ������ � ������ ��������������� ������ � ������:
   a. ������������ �� ������ � ����������� �� ��������� �����.
   b. ���� ��������� �������� ����, �������� ������ � �������� ����� � ������� ������� ���� � ����� ������.
*/

int Codec::DeCompressFile(ifstream& inputFileStream, ofstream& outputFileStream) {
	// 1. ��������� �� �������� ������ ���������� ����� ��������
	int numberOfHuffmanCodes;
	inputFileStream.read(reinterpret_cast<char*>(&numberOfHuffmanCodes), sizeof(numberOfHuffmanCodes));

	// 2. ������� ������ HC ��� �������� ����� ��������
	HC huffmanCodeTable;

	// 3. ��������� � ��������� ���� �������� �� �������� ������
	for (int i = 0; i < numberOfHuffmanCodes; ++i) {
		char symbol;
		unsigned char codeLength;
		unsigned int codeBits;

		// a. ��������� ������, ����� ���� � ���� ���� ����
		inputFileStream.get(symbol);
		inputFileStream.get(reinterpret_cast<char&>(codeLength));
		inputFileStream.read(reinterpret_cast<char*>(&codeBits), sizeof(codeBits));

		// b. ��������� ��������� ������ � ������� HC
		huffmanCodeTable.SetCode(HC::Code(codeLength, codeBits, symbol));
	}

	// 4. ������������ ������ �������� �� ���������� �����
	auto rootNode = huffmanCodeTable.ReconstructTree();
	auto currentNode = rootNode;

	// 5. ��������������� ������ ���� �� �������� ������ � ������ ��������������� ������ � ������
	int bit;
	while (getBit(inputFileStream, bit)) {
		// a. ������������ �� ������ � ����������� �� ��������� �����
		currentNode = (bit == 1) ? currentNode->right : currentNode->left;

		// b. ���� ��������� �������� ����, �������� ������ � �������� ����� � ������� ������� ���� � ����� ������
		if (currentNode->IsLeave()) {
			outputFileStream.put(currentNode->ch);
			currentNode = rootNode;
		}
	}

	return 0;
}


