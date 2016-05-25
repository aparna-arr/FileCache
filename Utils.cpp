#include "Utils.h"
using namespace std;

void usage(void)
{
	cout << "usage: cache -c <CACHE ROOT PATH> -i <WIGFILE>" << endl;
}

void handle_opts(int argc, char * argv[], string &cache_root, string &filename)
{
	int curr_opt;
	bool has_i = false, has_c = false;
	
	while ( (curr_opt = getopt(argc, argv, "i:c:")) != -1)
		switch(curr_opt)
		{
			case 'i':
				filename = optarg;
				has_i = true;
				break;
			case 'c':
				cache_root = optarg;
				has_c = true;
				break;
			case '?':
				if (optopt == 'i')
					throw invalid_argument ("-i requires a STRING argument!");
				else if (optopt == 'c')
					throw invalid_argument ("-c requires a STRING argument!");
				else
					throw invalid_argument ("Unrecognized argument!");	
	
				break;
			default:
				throw invalid_argument ("Something went wrong: default switch case");	
		}
	
	if (!has_i)
		throw invalid_argument ("option -i required!");

	if (!has_c)
		throw invalid_argument ("option -i required!");
}	
