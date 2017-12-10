#include <iostream>
#include <unistd.h>
#include <time.h>
using namespace std;
int main()
{
	int bbb=100;
	while(true)
	{
		usleep(100000);
		cout << time(0) << endl;
	}
}
