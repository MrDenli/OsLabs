#include<iostream>
#include<cmath>

using namespace std;
// extern позволяет компилятору знать о типах и именах глобальных переменных без действительного создания этих переменных
extern "C" float SinIntegral(float a, float b, float e) 
{
	float square = 0;
	for (float i = a; i <= b; i += e) {
		square += e * sin(i);
	}
	return square;
}


extern "C" char* translation(long x)
{	
	if (x == -1) { 
		cout<<"binary";
	}
	int cnt = 0;
	int sizelong = 31;
	char* binary = new char[sizelong];
	for (int i = 0; i < sizelong; i++) {
		binary[i] = '9';
	}
	while(x > 0) {
		if (x%2 == 1) {
			binary[sizelong - cnt - 1] = '1';
		} else {
			binary[sizelong - cnt - 1] = '0';
		}
		x = x/2;
		cnt++;
	}
	return binary;
}
