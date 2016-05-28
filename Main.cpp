#include "Utils.h"
#include "Test.h"
using namespace std;

int main (int argc, char * argv[])
{
	string cache_root, filename;

	try
	{
		handle_opts(argc, argv, cache_root, filename);	
	}
	catch (const invalid_argument &e)
	{
		return 1;
	}

	Test * testCache = new Test();

	testCache->test_all();

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

	bool deleteTest = myCache->clear_cache();
	
	if (!deleteTest)
		return 1;
	
	cout << "DeleteTest for clear_cache(): " << deleteTest << endl;
}	
