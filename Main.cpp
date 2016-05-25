#include "Utils.h"
using namespace std;

int main (int argc, char * argv[])
{
	string cache_root, filename;

	try
	{

		handle_opts(argc, argv, cache_root, filename);	

		cerr << "Your cache root is [" << cache_root << "], your filename is [" << filename << "]" << endl; 

	}
	catch (const invalid_argument &e)
	{
		cerr << "INVALID_ARGUMENT_ERROR: " << e.what() << endl << endl;
		usage();
		return 1;
	}

	Cache * myCache = new Cache(cache_root, filename);
	myCache->check_cache();
}	
