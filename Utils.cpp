#include "Utils.h"
using namespace std;

void usage(void)
{
	cout << "usage: cache [-d <DEBUG_LEVEL 0..3>] -c <CACHE ROOT PATH> -i <WIGFILE>" << endl;
}

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
