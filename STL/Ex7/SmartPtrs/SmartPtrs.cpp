#include <iostream>
#include <memory>
#include "Test.h"

using namespace std;

// Функция создает объект Test с помощью shared_ptr
shared_ptr<Test> foo() {
    shared_ptr<Test> pt(new Test);
    pt->Val = 69; // Изменяем значение поля Val
    cout << "In foo(), Val: " << pt->Val << endl;
    return pt; // Возвращаем умный указатель
}

// Функция создает объект Test с помощью обычного указателя
Test* goo() {
    Test* pt = new Test;
    pt->Val = 420; // Изменяем значение поля Val
    return pt; // Возвращаем обычный указатель
}

int main() {
    shared_ptr<Test> t; // Переменная для хранения умного указателя

    // Блок, в котором используется умный указатель, возвращаемый из foo()
    {
        shared_ptr<Test> p = foo(); // Получаем умный указатель от функции foo()
        t = p; // Присваиваем его переменной t
        cout << "In block, p->Val: " << p->Val << endl;
        cout << "In block, t.use_count(): " << t.use_count() << endl;
    }

    // После выхода из блока переменная p уничтожается
    cout << "After block" << endl;
    cout << "t.use_count(): " << t.use_count() << endl; // Счетчик ссылок t
    cout << "t->Val: " << t->Val << endl; // Значение поля Val объекта Test, на который указывает t

    // Пример использования функции goo(), возвращающей обычный указатель
    shared_ptr<Test> p2(goo()); // Создаем shared_ptr из обычного указателя
    cout << "p2->Val: " << p2->Val << endl;

    return 0;
}
