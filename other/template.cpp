#include <iostream>
#include <typeinfo>
using namespace std;

class A{
public:
	template<typename T>void foo(void){
		cout << typeid(T).name() << endl;
	}
};
template<typename T>void foo(T& r,T *p){
	r.foo<int>();p->foo<int>();
}
int main(){
	
A a;foo(a,&a);
}
