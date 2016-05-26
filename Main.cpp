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

	cerr << "Testing MD5sum functions" << endl;
	string md5test_str = "happy" ;
	cerr << "string is [" + md5test_str + "]" << endl;
	cerr << "md5 of string is [" << MD5_string(md5test_str) << "]" << endl;

	Cache * myCache = new Cache(cache_root, filename);
	bool checkTest = myCache->check_cache();
	
	cout << "CheckTest for check_cache(): " << checkTest << endl;

	if (!checkTest)
		return 1;	

	unordered_map<string, vector<Peak>> * dataTest;

	bool checkData = myCache->get_data(dataTest);

	cout << "CheckData for get_data(): " << checkData << endl;
	
	if (!checkData)
		return 1;
	
	for (auto chrIter = dataTest->begin(); chrIter != dataTest->end(); chrIter++)
	{
		cout << "CurrChr is " << chrIter->first << endl;
		for (vector<Peak>::iterator peakIter = (chrIter->second).begin(); peakIter != (chrIter->second).end(); peakIter++)
		{
			cout << "\tstart: " << peakIter->start << ", end: " << peakIter->end << ", value: " << peakIter->value << endl;
		}
	}	
}	
