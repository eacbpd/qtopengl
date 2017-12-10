#include <iostream>
using namespace std;
class A{
public:
int a=0;
virtual void print()
{
	cout << "Im A"<< endl;
}
};

class B:public A{
public:
int b=0;
virtual void print()
	{
		cout << "Im B" << endl;
	}
};

class C:public B{
public:
int c=0;
void print(){cout << "Im C" << endl;}
};

class D:public C{};

class E
{
int a;
char b;
int C;
};

class F{virtual void fun();};

void unnfun(){cout << "alb";}

int main()
{
	A a,*pa;
	B b,*pb;
	C c,*pc;
	cout << &a << '\n' << &b << '\n' << &c << endl;
	a.print();
	b.print();
	c.print();
	pa = &c;
	pa->print();
	cout << sizeof(A) << '\n'
	<< sizeof(B) << '\n'
	<< sizeof(C) << '\n' 
	<< sizeof(D) << '\n'
	<< sizeof(E) << '\n'
	<< sizeof(pa) << '\n'
	<< endl;
	void (*pfun)();
	pfun = &unnfun;
	pfun();
}
