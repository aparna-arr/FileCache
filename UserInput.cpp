#include"UserInput.h"
using namespace std;

void usage(void)
{
	cout << "usage: cache [-d <DEBUG_LEVEL 0..3>] -c <CACHE ROOT PATH> -i <WIGFILE>" << endl;
}


void handle_opts(int argc, char * argv[], std::string &cache_root, std::string &filename, bool & remove, bool & remove_all)
{
	try 
	{
	int curr_opt;
	bool has_i = false, has_c = false;
	remove = false;
	remove_all = false;
	
	while ( (curr_opt = getopt(argc, argv, "i:c:d:rf")) != -1)
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
			case'd':
			{
				stringstream stream(optarg);
				int debugopt;

				if (!(stream>>debugopt))
					throw invalid_argument("-d requres an INT argument!");
				DebugLevel(debugopt);
			}
				break;
			case'r':
				remove = true;
				break;
			case'f':
				remove_all = true;
				break;
			case '?':
				if (optopt == 'i')
					throw invalid_argument ("-i requires a STRING argument!");
				else if (optopt == 'c')
					throw invalid_argument ("-c requires a STRING argument!");
				else if (optopt == 'd')
					throw invalid_argument ("-d requires an INT argument!");
				else
					throw invalid_argument ("Unrecognized argument!");	
	
				break;
			default:
				throw invalid_argument ("Something went wrong: default switch case");	
		}
	
	if (!has_i && !remove_all)
		throw invalid_argument ("option -i required!");

	if (!has_c)
		throw invalid_argument ("option -c required!");
	}
	catch (const invalid_argument &e)
	{
		cerr << "INVALID_ARGUMENT_ERROR: " << e.what() << endl << endl;
		usage();
		throw;
	}
}

