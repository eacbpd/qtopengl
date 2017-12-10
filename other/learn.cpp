#include <iostream>
#include <typeinfo>
using namespace std;

class A{

public:
	virtual void fun(){cout << "fun a" << endl;}
	short st;
	char c;
	int it;
};

class B:public A{
public:
	virtual void fun(){cout << "fun b" << endl;}
};

int main(int argc, char *argv[])
{
	A a,*pa;
	B b,*pb;

	cout << sizeof(a) << endl;
	cout << sizeof(b) << endl;
	a.fun();
	pa = &b;
	pa->fun();
	cout << typeid(int).name() << endl;
	cout << typeid(pa).name() << endl;
	cout << typeid(a).name() << endl;
	//b.fun();
	return 0;
}


