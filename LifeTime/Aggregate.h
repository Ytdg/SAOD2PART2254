#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Aggregate
{
	T m_objTest;
public:

	Aggregate()
	{
		cout << "Create Agr" << endl;
	};

	~Aggregate()
	{
		cout << "Delete Agr" << endl;
	}

};
