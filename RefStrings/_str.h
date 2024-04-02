#pragma once
#include <string.h>
class Str;
class _str
{
	char* m_pszData;
	int m_nCount;
	friend class Str;	//Дружественный класс или метод
	_str() {
		m_pszData = new char[1] {0}; m_nCount = 1;
	}
	_str(const char* p) {
		m_nCount = 1;
		m_pszData = new char[strlen(p) + 1];
		strcpy(m_pszData, p);
	}
	~_str() { delete[]m_pszData; }
	void AddRef() { m_nCount++; } 	//Удаляет себя v
	void Release() { m_nCount--; if (m_nCount == 0) delete this; }
};


