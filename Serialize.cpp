#include"Serialize.h"
using namespace std;

WigCache::WigCache(std::string fullpath)
{
	debug("WigCache::WigCache(): begin", 1);
	path = fullpath;
	debug("WigCache::WigCache(): end", 1);
}

bool WigCache::serialize(std::string file)
{
	try
	{
		debug("WigCache::serialize(): begin", 1);
		unordered_map<string, vector<Peak>> * peaks_map = new unordered_map<string, vector<Peak>>;

		readInWig(file, peaks_map);
		
		for (auto iter = peaks_map->begin(); iter != peaks_map->end(); iter++)
		{
			debug("WigCache::serialize(): chromosome: " + iter->first, 2);
			int numPeaks = (iter->second).size();
			Peak * peaks_array;
			peaks_array = new Peak[numPeaks];

			vectorToArray((iter->second), peaks_array);
			
			flatten(peaks_array, numPeaks, path + iter->first + ".dat");
			delete [] peaks_array;
		}

		delete peaks_map;
		debug("WigCache::serialize(): end", 1);
		return true;
	}
	catch (const runtime_error &e)
	{
		cerr << "WigCache: serialize(): RUNTIME_ERROR: " << e.what() << endl;
		return false;
	}

	return false;
}

bool WigCache::deserialize(std::unordered_map<std::string, std::vector<Peak>> *& data)
{
	try
	{
		debug("WigCache::deserialize(): begin", 1);
		DIR * dirp;
		struct dirent * ds;
	
		if ((dirp = opendir(path.c_str())) == NULL)
			throw runtime_error("deserialize(): DIR [" + path + "] cannot be opened!");

		size_t found;
		vector<string> chrfiles;

		data = new unordered_map<string, vector<Peak>>;

		while((ds = readdir(dirp)) != NULL)
			if ((found = string(ds->d_name).find(".dat")) != string::npos)
				chrfiles.push_back(ds->d_name);

		if (chrfiles.size() == 0)
		{
			delete data;
			throw runtime_error("deserialize(): DIR [" + path+ "] contains no .dat files!");
		}

		for (vector<string>::iterator iter = chrfiles.begin(); iter != chrfiles.end(); iter++)
		{
			string curr_chr = (*iter).substr(0,(*iter).find(".dat"));
			Peak * peakAr;
			int numPeaks;
			unflatten(peakAr, numPeaks, path + "/" + *iter);
			
			arrayToVector(peakAr, numPeaks, (*data)[curr_chr]);
		}

		debug("WigCache::deserialize(): end", 1);
		return true;
	}
	catch (const runtime_error &e)
	{
		cerr << "WigCache: deserialize(): RUNTIME_ERROR: " << e.what() << endl;
		return false;
	}

	return false;
}

bool WigCache::readInWig(std::string filename, std::unordered_map<std::string, std::vector<Peak>> *& peaks)
{
	debug("WigCache::readInWig(): begin", 1);
	ifstream fp(filename);

	if (!fp.is_open())
	{
		throw runtime_error("readInWig(): Cannot open file [" + filename + "]");
		return false;
	}

	string line;

	string curr_chr = "INIT";
	int span;

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
				curr_chr = chr;
		}
		else if (ss >> pos >> value)
		{
			Peak tmp;
			tmp.start = pos;
			tmp.end = pos + span;
			tmp.value = value;
	
			(*peaks)[curr_chr].push_back(tmp);
		}
	}

	if ((*peaks).empty())	
		throw runtime_error("readInWig(): Peaks are empty!");

	debug("WigCache::readInWig(): end", 1);
	return true;
}

bool WigCache::vectorToArray(std::vector<Peak> & peaks_vec, Peak *& peaks_ar) 
{
	debug("WigCache::vectorToArray(): begin", 1);
	vector<Peak>::iterator it = peaks_vec.begin();
	int i = 0;

	while (it != peaks_vec.end())
	{
		peaks_ar[i] = *it;
		it++;
		i++;
	}
		
	peaks_vec.erase(peaks_vec.begin(), peaks_vec.end());

	debug("WigCache::vectorToArray(): end", 1);
	return true;
}

bool WigCache::arrayToVector(Peak * peaks_ar, int numPeaks, vector<Peak> & peaks_vec)
{
	debug("WigCache::arrayToVector(): begin", 1);
	for (int i = 0; i < numPeaks; i++)
		peaks_vec.push_back(peaks_ar[i]);
	
	// delete peaks_ar ???
	debug("WigCache::arrayToVector(): end", 1);
	return true;
}

bool WigCache::flatten(Peak * peaks_ar, int numPeaks, std::string file)
{
	debug("WigCache::flatten(): begin", 1);
	ofstream out(file.c_str(), ios::binary);

	if (!out.is_open())
		throw runtime_error("flatten(): cannot open outfile [" + file + "]");		

	out.write(reinterpret_cast<char*>(&numPeaks), sizeof(int));
	
	for (int i = 0; i < numPeaks; i++)
		out.write(reinterpret_cast<char*>(&(peaks_ar[i])), sizeof(Peak));

	out.close();

	debug("WigCache::flatten(): end", 1);

	return true;
}

bool WigCache::unflatten(Peak *& peaks_ar, int & numPeaks, std::string file)
{
	debug("WigCache::unflatten(): begin", 1);
	ifstream in(file.c_str(), ios::binary);
	
	if (!in.is_open())
		throw runtime_error("unflatten(): cannot open infile [" + file + "]");

	in.read(reinterpret_cast<char*>(&numPeaks), sizeof(int));

	peaks_ar = new Peak[numPeaks];

	for (int i = 0; i < numPeaks; i++)
		in.read(reinterpret_cast<char*>(&(peaks_ar[i])), sizeof(Peak));
	
	in.close();

	debug("WigCache::unflatten(): end", 1);
	return true;
}
