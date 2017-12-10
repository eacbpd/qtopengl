#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>
using namespace std;

class writer{
		ofstream ofs;
		mutex mut;
public:
		writer(){
				ofs.open("./atxt");
		}
		~writer(){
				ofs.close();
		}
		void shprint(string id,int &val){
				lock_guard<mutex> locker(mut);
				ofs<<id<<' '<<++val<<endl;
		}
};

void fun(writer &w,int &r){
//		while(++r<100)
		for(int i=0;i!=100;++i)
//		while(r<100)
				w.shprint("fun", r);
}	

int main(){
		int m=0;
		writer w;
		thread t1(fun,ref(w),ref(m));
//		while(++i<100)
		for(int i=0;i!=100;++i)
//		while(i<100)
				w.shprint("man", m);
		t1.join();
		cout <<thread::hardware_concurrency()<< endl;
		return 0;
}
