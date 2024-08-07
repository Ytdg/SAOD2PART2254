#pragma once

#include <iostream>

using namespace std;

// ����� �������-�������
class PF {
public:
    int* pv; // ��������� �� ������������ ������ ��� �������� �������� �������-�������
    int n; // ����� ������, ��� ������� ����������� �������-�������
    int cmpCount; // ������� ��������� �������� ��� ������� ��������� ���������

    // �����������, ����������� ������ � ���������������� �������-�������
    PF(const char* string) {
        cmpCount = 0;
        Init(string);
    }

    // ����������, ������������� ������������ ������, ���������� ��� �������
    ~PF() {
        delete[] pv;
    }

    //  ����������� �����������
    PF(const PF& other) {
        n = other.n;
        pv = new int[n];
        memcpy(pv, other.pv, n * sizeof(int));
        cmpCount = other.cmpCount;
    }

    //  ���������� ��������� ������������
    PF& operator=(const PF& other) {
        if (this != &other) {
            delete[] pv;
            n = other.n;
            pv = new int[n];
            memcpy(pv, other.pv, n * sizeof(int));
            cmpCount = other.cmpCount;
        }
        return *this;
    }

    // �������� �������������� ���� ��� ��������� ����� ������
    operator int() const {
        return n;
    }

    // �������� ���������� ��� ��������� �������� �������-������� �� �������
    int operator[](int i) const {
        if (i >= n || i < 0) throw out_of_range("Index out of range"); // �������� ������ ������� �� �������
        return *(pv + i); // ���������� �������� �������-������� ��� ������� �������
    }

    // ����� ��� ��������� ���������� ��������� ��������
    int CmpCount() {
        return cmpCount;
    }

private:
    // ����� ��� ������������� �������-�������
    void Init(const char* string) {
        n = strlen(string); // ���������� ����� ������
        pv = new int[n]; // ��������� ������������ ������ ��� ������� �������-�������

        *pv = 0; // ������ �������� �������-������� ������ ����� 0

        for (int i = 1; i < n; i++) {
            int j = pv[i - 1]; // ��������� �������� ��� ������ ����������� ������������ ��������/��������

            // ����� ����������� ������������ ��������/��������
            while (j > 0 && string[i] != string[j]) {
                j = pv[j - 1]; // ������� � ����������� �������� �������-�������
                cmpCount++; // ���������� �������� ��������� ��������
            }

            // ���� ������� ���������� ��������, ����������� �������� �������-�������
            if (string[i] == string[j]) j++;

            pv[i] = j; // ���������� �������� �������-������� ��� �������� �������
        }
    }
};