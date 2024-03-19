#pragma once
#include <iostream>
#include "Test.h"

using namespace std;

template <class T>
class AggregateT {
public:
	T* m_objTest;
	AggregateT() {
		cout << "	T  create! " << endl;
		m_objTest = new T();
	}

	~AggregateT() {
		cout << "	T remove! " << endl;
		delete m_objTest;
	}
};

