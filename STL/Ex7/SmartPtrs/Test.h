#ifndef TEST_H
#define TEST_H

#include <iostream>
using namespace std;

//  ласс Test с "говор€щими" конструктором и деструктором
class Test {
public:
    int Val;

    //  онструктор выводит сообщение и инициализирует Val нулем
    Test() : Val(0) {
        cout << "Created" << endl;
    }

    // ƒеструктор выводит сообщение
    ~Test() {
        cout << "Destroyed" << endl;
    }
};

#endif // TEST_H
