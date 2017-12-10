#include <iostream>
using namespace std;
int main()
{
	unsigned int i=0x01020304;
	unsigned char *c =(unsigned char *)&i;
	cout << int(*c++) << endl;
	cout << int(*c++) << endl;
	cout << int(*c++) << endl;
	cout << int(*c++) << endl;
	cout << sizeof(int )<< endl;
	short x;
	char x0;
	x=0x0102;
	x0=((char *)&x)[0];
	cout << int(x0) << endl;
}
