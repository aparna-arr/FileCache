#include"Debug.h"
using namespace std;

int DebugLevel(int debug_init) // this can only be initialized once
{
	static int debugValue = debug_init;
	return debugValue;
}

void debug (std::string err, int debug_lvl)
{
	if (debug_lvl <= DebugLevel())
		cerr << err << endl;
}
