#ifndef TEST_H
#define TEST_H

#include <iostream>
using namespace std;

// ����� Test � "����������" ������������� � ������������
class Test {
public:
    int Val;

    // ����������� ������� ��������� � �������������� Val �����
    Test() : Val(0) {
        cout << "Created" << endl;
    }

    // ���������� ������� ���������
    ~Test() {
        cout << "Destroyed" << endl;
    }
};

#endif // TEST_H
