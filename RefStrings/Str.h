#pragma once
#include "_str.h"
class Str {
	_str* m_pStr;
public:
	Str() { m_pStr = new _str; }		
	Str(const char* p) {		
		m_pStr = new _str(p);
	}
	~Str() {
		m_pStr->Release(); 	
	}				
	Str(const Str& s)
	{
		m_pStr = s.m_pStr; 	// ссылаемс€ на тот же ресурс
		m_pStr->AddRef(); 	// отмечаем, что сослались
	}

	int len() const {
		return strlen(m_pStr->m_pszData);
	}
	Str& operator = (const Str& s) {
		if (this != &s) {
			s.m_pStr->AddRef(); 
			m_pStr->Release();
			m_pStr = s.m_pStr;
		}
		return *this;
	}
	
	int find(const char* t, int off) const {
		int indexSubStr = 0;
		int findIndex = -1;
		while (off<len()&&indexSubStr<strlen(t))
		{
			if (m_pStr->m_pszData[off]==t[indexSubStr])
			{
				if (findIndex == -1) { findIndex = off; }
				indexSubStr++;
			}
			else{
				findIndex = -1;
				indexSubStr = 0;
			}
			off++;
		}
		return findIndex;
	}
	operator const char* ()const { return m_pStr->m_pszData; }
	Str& operator += (const Str& s) {
		int length = len() + s.len();
		if (s.len() != 0) {		// добавление ничего не изменит!
			_str* pstrTmp = new _str; 	// Ќовый ресурс
			delete[] pstrTmp->m_pszData;

			pstrTmp->m_pszData = new char[length + 1];
			strcpy(pstrTmp->m_pszData,m_pStr->m_pszData);
			strcat(pstrTmp->m_pszData,s.m_pStr->m_pszData);

			m_pStr->Release();
			m_pStr = pstrTmp;
		}
		return *this;
	}

};

