#include "Test.h"
int Test::Count;
extern Test publciTest;



Test::Test()
{
	Count++;
	cout << "Create Test" << Count << endl;
};
Test::~Test()
{
	cout << "Remove Test" << Count << endl;
	Count--;
}
Test::Test(const Test& test)
{
	Count++;
	cout << "Copy" << endl;
};
void Test::Foo(Test test)
{
	cout << "void Foo" << endl;
};

