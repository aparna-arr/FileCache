#include"Test.h"
#include<algorithm>
using namespace std;

Test::Test()
{
	testwig = "TEST_WIG_FILE.wig";
	test_str = "whatismymd5";
}

Test::~Test()
{
	remove(testwig.c_str());
}

bool Test::test_all(void)
{
	cout << GREEN << "*** Begin Test Suite ***" << DEFAULT << endl;

	if (!test_md5())
		return false;
	if (!test_serialize())
		return false;
	if (!test_cache())
		return false;

	cout << endl << GREEN << "*** End Test Suite ***" << DEFAULT << endl;
	return true;
}

bool Test::test_md5(void)
{
	cout << endl<< GREEN << "** Testing MD5 functions **" << DEFAULT << endl;
	cout << endl << RED << "MD5_string()" << DEFAULT << endl;
	cout << "\t- test string is [" << test_str << "]" << endl;

	string cmd = "printf \"\%s\" " + test_str + " | md5sum";
	FILE * md5 = popen(cmd.c_str(), "r");
	char buf[MD5_DIGEST_LENGTH*2+1];
	fgets(buf, sizeof(buf), md5);
	pclose(md5);

	cout << "\t- md5 of test string is [" << string(buf) << "]" << endl;
	
	string testout = MD5_string(test_str);
	
	cout << "\t- MD5_string() is [" << testout << "]" << endl;
	 
	if (testout != string(buf))
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}	

	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << endl <<RED << "MD5_file()" << DEFAULT << endl;

	create_file();

	cout << "\t- test filename is [" << testwig << "]" << endl;
	
	string cmd2 = "md5sum " + testwig;
	FILE * md5_file = popen(cmd2.c_str(), "r");
	char buf2[MD5_DIGEST_LENGTH*2+1];
	fgets(buf2, sizeof(buf), md5_file);
	pclose(md5_file); 

	cout << "\t- md5 of test file is [" << string(buf2) << "]" << endl;
	string testout_2 = MD5_file(testwig);

	cout << "\t- MD5_file() is [" << testout_2 << "]" << endl;

	if (testout_2 != string(buf2))
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}
	
	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << endl << RED << "compare_file_MD5()" << DEFAULT << endl;
	cout << "\t- md5sum is [" << testout_2 << "]" << endl;
	cout << "\t- unmodified file" << endl;
	
	bool testout_3 = compare_file_MD5(testout_2, testwig); 

	cout << "\t- compare_file_MD5(): [" << testout_3 << "]" << endl;
	
	if (!testout_3)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	cout << RED << "\tPASS" << DEFAULT << endl;

	modify_file();

	cout << "\t- modified file" << endl;

	bool testout_4 = compare_file_MD5(testout_2, testwig);

	cout << "\t- compare_file_MD5(): [" << testout_4 << "]" << endl;
	
	if (testout_4)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	cout << RED << "\tPASS" << DEFAULT << endl;
	cout << endl << GREEN << "PASS" << DEFAULT << endl;

	return true;
}

bool Test::test_serialize(void)
{
	string path = "TEST_SERIALIZE/";
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	
	cout << endl << GREEN << "** Testing Serialize (WigCache) functions **" << DEFAULT << endl;
	cout << endl << RED << "serialize()" << DEFAULT << endl;

	cout << "\t- path is [" << path << "]" << endl;
	cout << "\t- path is [" << testwig << "]" << endl;
	cout << "\t- creating new WigCache" << endl;

	WigCache * wig_1 = new WigCache(path);
	
	bool test_serialize = wig_1->serialize(testwig);
	
	cout << "\t- serialize() is [" << test_serialize << "]" << endl;

	if (!test_serialize)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete wig_1;

	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << endl << RED << "deserialize()" << DEFAULT << endl;

	cout << "\t- creating new WigCache" << endl;

	WigCache * wig_2 = new WigCache(path);

	unordered_map<string, PeakInfo> * data = new unordered_map<string,PeakInfo>;

	bool test_deserialize = wig_2->deserialize(data);

	cout << "\t- deserialize is [" << test_deserialize << "]" << endl;
	
	if (!test_deserialize)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete wig_2;
	
	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << "\t- compare deserialize peaks to file peaks" << endl;
	cout << "\t- file peaks are:" << endl;
	
	string file_peaks, deserialize_peaks;
	
	file_peaks = file_cat();

	cout << file_peaks << endl;

	cout << "\t- deserialize peaks are:" << endl;

	deserialize_peaks = datastructure_to_string(data);
	
	cout << deserialize_peaks << endl;

	DIR * dirp;
	dirp = opendir(path.c_str());
	struct dirent * ds;
	size_t found;

	while((ds = readdir(dirp)) != NULL)
	{
		if ((found = string(ds->d_name).find(".dat")) != string::npos)
		{
			string curr_file = path + string(ds->d_name);
			remove(curr_file.c_str());
		}
	}
	
	rmdir(path.c_str());

	if (deserialize_peaks != file_peaks)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	cout << RED << "\tPASS" << DEFAULT << endl;
	cout << endl << GREEN << "PASS" << DEFAULT << endl;
		
	return true;
}

bool Test::test_cache(void)
{
	cout << endl << GREEN << "** Testing Cache functions **" << DEFAULT << endl;
	cout << endl << RED << "check_cache()" << DEFAULT << endl;	

	cout << "\t- creating new Cache object" << endl;
		
	string cacheRoot = "TEST_CACHE/";

	Cache * testCache = new Cache(cacheRoot, testwig);

	cout << "\t- testing check_cache()" << endl;
	
	bool checkCacheTest = testCache->check_cache();

	cout << "\t- check_cache(): [" << checkCacheTest << "]" << endl;
	
	if (!checkCacheTest)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}


	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << endl << RED << "check_cache() with modified file" << DEFAULT << endl;	

	cout << "\t- creating new Cache object" << endl;
		
	Cache * testCache2 = new Cache(cacheRoot, testwig);

	cout << "\t- modifying file" << endl;

	modify_file();

	cout << "\t- testing check_cache()" << endl;
	
	bool checkCacheTest2 = testCache2->check_cache();

	cout << "\t- check_cache(): [" << checkCacheTest2 << "]" << endl;
	
	if (!checkCacheTest2)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete testCache2;

	cout << RED << "\tPASS" << DEFAULT << endl;

	unordered_map<string, PeakInfo> * data = new unordered_map<string, PeakInfo>;

	cout << endl << RED << "get_data() from same instance" << DEFAULT << endl;

	bool checkDataTest = testCache->get_data(data);

	if (!checkDataTest)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete testCache;

	cout << "\t- testing if peaks are the same as file peaks" << endl;

	string data_1 = datastructure_to_string(data);
	
	if (data_1 != file_cat())
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}


	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << endl << RED << "get_data() from different instance" << DEFAULT << endl;

	unordered_map<string, PeakInfo> * data2 = new unordered_map<string, PeakInfo>;
	
	Cache * testCache3 = new Cache(cacheRoot, testwig);	

	bool checkDataTest2 = testCache3->get_data(data2);

	if (!checkDataTest2)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete testCache3;
	cout << "\t- testing if peaks are the same as file peaks" << endl;

	string data_2 = datastructure_to_string(data);
	
	if (data_2 != file_cat())
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete data;
	delete data2;

	cout << RED << "\tPASS" << DEFAULT << endl;
	
	cout << endl << RED << "rm_file_cache()" << DEFAULT << endl;

	cout << "\t- creating new cache" << endl;
	Cache * testCache4 = new Cache(cacheRoot, testwig);

	cout << "\t- removing file [" << testwig << "]" << endl;
	bool testRmFileCache = testCache4->rm_file_cache();
	cout << "\t- rm_file_cache() is [" << testRmFileCache << "]" << endl;

	if (!testRmFileCache)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete testCache4;

	cout << RED << "\tPASS" << DEFAULT << endl;
		
	cout << "\t- creating new cache" << endl;
	
	Cache * testCache5 = new Cache(cacheRoot, testwig);

	cout << "\t- checking new cache" << endl;
		
	testCache5->check_cache();

	cout << "\t- clearing cache" << endl;
	
	bool testClear = testCache5->clear_cache();

	cout << "\t- clear_cache() is [" << testClear << "]" << endl;

	if (!testClear)
	{
		cout << RED << "\tFAIL" << DEFAULT << endl;
		return false;
	}

	delete testCache5;

	cout << RED << "\tPASS" << DEFAULT << endl;

	cout << endl << GREEN << "PASS" << DEFAULT << endl;
	
	return true;
}

void Test::create_file(void)
{
	string str;
	str += "variableStep chrom=chr1 span=99\n";
        str += "100\t10\n";
        str += "300\t20\n";
        str += "variableStep chrom=chr1 span=199\n";
        str += "400\t10\n";
        str += "600\t5\n";
        str += "variableStep chrom=chr2 span=99\n";
        str += "300\t1\n";
        str += "400\t2\n";
        str += "variableStep chrom=chr2 span=499\n";
        str += "500\t40\n";
        str += "600\t5\n";

	ofstream file(testwig.c_str());	
	file << str;
	file.close();
}

void Test::modify_file(void)
{
	fstream file;
	file.open(testwig.c_str(), ios::out | ios::app);
	file << "2000\t80\n";
	file.close();
}

string Test::file_cat(void)
{
	ifstream fp(testwig.c_str());
	unordered_map<string, PeakInfo> * peaks = new unordered_map<string, PeakInfo>;
	string line;

        string curr_chr = "INIT";
        int span;

	vector<Peak> * curr_peaks = new vector<Peak>;
        while(getline(fp, line) && !fp.bad())
        {
                stringstream ss(line);

                int pos;
                double value;

                if (line.find("variableStep") == 0)
                {
                        string chr, str;

                        ss >> str; // variableStep 
                        ss >> str; // chrom
                        size_t found = str.find("=");

                        chr = str.substr(found+1);

                        ss >> str; // span
                        found = str.find("=");
                        stringstream spanstr(str.substr(found+1));

                        spanstr >> span;

                        if (chr != curr_chr)
			{
				(*peaks)[curr_chr].numPeaks = curr_peaks->size();
				(*peaks)[curr_chr].peaks = new Peak[(*peaks)[curr_chr].numPeaks];
				for (unsigned int i = 0; i < (*peaks)[curr_chr].numPeaks; i++)
					(*peaks)[curr_chr].peaks[i] = (*curr_peaks)[i];

				curr_peaks->erase(curr_peaks->begin(), curr_peaks->end());
                                curr_chr = chr;
			}
             
         }
                else if (ss >> pos >> value)
                {
                        Peak tmp;
                        tmp.start = pos;
                        tmp.end = pos + span;
                        tmp.value = value;

                        (*curr_peaks).push_back(tmp);
                }
        }

	(*peaks)[curr_chr].numPeaks = curr_peaks->size();
	(*peaks)[curr_chr].peaks = new Peak[(*peaks)[curr_chr].numPeaks];

	for (unsigned int i = 0; i < (*peaks)[curr_chr].numPeaks; i++)
		(*peaks)[curr_chr].peaks[i] = (*curr_peaks)[i];

	curr_peaks->erase(curr_peaks->begin(), curr_peaks->end());
	delete curr_peaks;

	return datastructure_to_string(peaks);
}

void insertionSort(PeakInfo & array)
{
	unsigned int size = array.numPeaks;
	
	unsigned int j;
	for (unsigned int i = 1; i < size; i++)
	{
		Peak tmp = array.peaks[i];
		j = i - 1;
	
		while (j >= 0 && array.peaks[j].start > tmp.start)
		{
			array.peaks[j+1] = array.peaks[j];
			j--;
		}
		array.peaks[j+1] = tmp;
	}
}

string Test::datastructure_to_string(unordered_map<string, PeakInfo> *& data)
{
	vector<string> chrlist;
	string outstring = "";

	for (auto iter = (*data).begin(); iter != (*data).end(); iter++)
	{
		chrlist.push_back(iter->first);
		insertionSort(iter->second);
	}

	sort (chrlist.begin(), chrlist.end());

	for (vector<string>::iterator it = chrlist.begin(); it != chrlist.end(); it++)
		for (unsigned int i = 0; i < (*data)[*it].numPeaks; i++)
		{
			stringstream start_str, end_str, value_str;
			start_str << (*data)[*it].peaks[i].start;
			end_str << (*data)[*it].peaks[i].end;
			value_str << (*data)[*it].peaks[i].value;

			outstring += *it + "\t" + start_str.str() + "\t" + end_str.str() + "\t" + value_str.str() + "\n";
		}

	return outstring;
}
