#pragma once
class Str
{
public:
	char* m_pszText;

	Str() 	//Конструктор “по умолчанию” (default)*
	{
		m_pszText = new char[1] {0};
	}
	Str(const char* p)
	{
		if (p) {
			m_pszText = new char[strlen(p) + 1];
			strcpy_s(m_pszText, strlen(p) + 1, p);
		}
		else
			m_pszText = new char[1] {0};
	}
	operator const char* ()const { return m_pszText; }

	Str(const Str& s) //Константная ссылка на объект
	{
		m_pszText = new char[strlen(s.m_pszText) + 1];
		strcpy_s(m_pszText,
			strlen(s.m_pszText) + 1,
			s.m_pszText);
	}
	Str operator +(const char* sz) const {
		Str res = Str(sz);
		return  res;
	}
	Str& operator +=(const char* sz) {
		
		char* array = new char[strlen(m_pszText) + strlen(sz)+1];
		strcpy(array, m_pszText);
		strcat(array, sz);
		delete[]m_pszText;
		this->m_pszText = array;
		return  *this;
	};

	~Str() { delete[]m_pszText; }

};
 
