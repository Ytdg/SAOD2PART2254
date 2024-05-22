// Huffman.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Codec.h"

int main()
{
	/// Выбор моды и подготовка потоков чтения записи
	string sFInName;
	ofstream os;
	ifstream is;
	char cRead;
	cout << "Enter the first letter of the operation (c - compressing; d - decompressing):";
	cin >> cRead; cin.get();

	if (cRead != 'c' && cRead != 'd'){
		printf("Error: can't recognize this operation!..\n");
		return -1;
	}

	cout << "Enter name of Input File:" << endl;
	getline(cin, sFInName);


	// Открываем потоки для ввода и вывода
	is.open(sFInName, ios::binary);
	if (!is.is_open()){
		cout << "Error: can't open file with name " << sFInName << "!.." << endl;
		return -1;
	}

	string sFOutName;
	cout << "Enter name of Output File:" << endl;
	getline(cin, sFOutName);
	os.open(sFOutName, ios::binary);

	if (!os.is_open()){
		cout << "Error: can't open file with name " << sFOutName << "!.." << endl;
		return -1;
	}
	try {
		if (cRead == 'c')
			Codec::CompressFile(is, os);
		else if (cRead == 'd')
			Codec::DeCompressFile(is, os);
	}
	catch (exception &ex)
	{
		cout << ex.what() << endl;
	}
	return 0;
}
