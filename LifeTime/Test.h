#pragma once
#include <iostream>
using namespace std;
class Test
{
   
public:
    static int Count;
    Test();
    void Foo(Test test);
    ~Test();
    Test(const Test& test);
};

