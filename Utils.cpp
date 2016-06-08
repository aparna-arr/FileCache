#include "Utils.h"
using namespace std;

string conv(int num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

string conv(double num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

bool toInt(string str, int num)
{
	stringstream ss(str);
	
	return (ss >> num);
}	
