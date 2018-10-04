#include "hash.h"
#include <cmath>
#include <iostream>
using namespace std;

unsigned long long calculateHash(string mystring)
{
	/* add your code here */
	unsigned long long value = mystring[0];
	unsigned end = mystring.length() - 1;
	unsigned int w[4];

	if (mystring.empty())
	{
		return 0;
	}
	for (unsigned n = 0; n < end; n++)
	{
		value = value*128 + mystring[n+1];
	}

	for (int i = 0; i < 4; i++)
	{
		w[i] = value % 65521;
		value /= 65521;
	}
	value = (45912 * w[3] + 35511 * w[2] + 65169 * w[1] + 4625 * w[0]) % 65521;
	return value;
}