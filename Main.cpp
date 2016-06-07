#include "Cache.h"
#include "UserIn.cpp"
using namespace std;

int main (int argc, char * argv[])
{
	string cache_root, filename;

	try
	{
		bool remove, remove_all;
		handle_opts(argc, argv, cache_root, filename, remove, remove_all);
		if (remove_all)
		{
			Cache * myCacheRM = new Cache(cache_root);
			myCacheRM->clear_cache();

			delete myCacheRM;
		}
		else
		{
			Cache * myCache = new Cache(cache_root, filename);
			
			if (remove)
				myCache->rm_file_cache();
			else	
				myCache->check_cache();

			delete myCache;
		}	
	}
	catch (const invalid_argument &e)
	{
		return 1;
	}
}	
