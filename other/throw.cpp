#include <iostream>
#include <thread>
using namespace std;

void fun(int &a){
		cout << a*a << endl;
}

int main(){
		int a=1;
		thread t1(fun,ref(++a));
		t1.join();
		thread t2(fun,ref(++a));
		t2.join();
		thread t3(fun,ref(++a));
		t3.join();
		try{
				cout << "abc" << endl;
				throw "a";
		}
		catch(...)
		{
				cout << "throw" << endl;
		}
		return 0;
}
