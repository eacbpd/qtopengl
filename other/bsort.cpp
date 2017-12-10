#include <iostream>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sys/timeb.h>
using namespace std;
bool cmpint(int &p1, int &p2)
{
	return p1<p2;
}
int main(int argc, char *argv[])
{
	srand(time(0));
	vector<int> vi;
	vector<int> vj;
	for(int i=0;i!=10000;++i)
		vi.push_back(rand()%8192);
	vj=vi;

	struct timeb tb1;
	ftime(&tb1);
	printf("%d.%03d\n",(int)tb1.time,tb1.millitm);

	for(int i=vi.size();i!=1;--i)
		for(int j=0;j!=i;++j)
			if(vi[j]>vi[j+1])
				swap(vi[j],vi[j+1]);

	struct timeb tb2;
	ftime(&tb2);
	printf("%d.%03d\n",(int)tb2.time,tb2.millitm);

	sort(vj.begin(),vj.end(),cmpint);

	struct timeb tb3;
	ftime(&tb3);
	printf("%d.%03d\n",(int)tb3.time,tb3.millitm);

	short format=0;
	for(auto &a:vj)
	{
		if(++format==20)
		{
			cout << '\n';
			format=0;
		}
		cout << a << ' ';
	}
	return 0;
}

