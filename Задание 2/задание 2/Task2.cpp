// Task2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
using namespace std;
void Swap(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
    
}
int main()
{   /*
    typedef unsigned char byte;
    int x = -1;
    int *p = &x;
    byte* pb = (byte*)--p;		
    for (byte* pt = pb; pt - pb < sizeof(int); pt++)
        cout << (int)*pt << ' ';
    cout << endl;
    */
    int a = 3, b = 5;
    Swap(a, b);
    cout << a << b<<' ';
   
}