#include"Cache.h"
using namespace std;

// later functions DO NOT CHECK the validity of the root or filename afterconstructor: USE THIS ONLY IF YOU KNOW WHAT YOU ARE DOING
Cache::Cache(void)
{
	root = "INIT";
	filename = "INIT";
	new_cache = false;
	can_read_data = false;
}

Cache::Cache(string cache_root, string file)
{
	try
	{
		struct stat s;
		new_cache = false;
		can_read_data = false;

		if (stat(cache_root.c_str(), &s) == 0)
			if (S_ISDIR(s.st_mode))
				root = cache_root;
			else
				throw runtime_error("Path [" + cache_root + "] exists but is not a directory! Likely you have given the path of a FILE.");
		else // set flag to create a new cache dir whenever another function is called, throw errors for clear_cache() and rm_file_cache()
		{
			cerr << "Cache does not exist: will create new cache\n";
			new_cache = true;
			root = cache_root;
		}
		
		ifstream test(file.c_str());

		if (test)
			filename = file;
		else
			throw runtime_error("Cannot read file [" + file + "]: File does not exist or is unreadable!");
		
		test.close();
	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: Cache(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

// returns TRUE if data is successfully gotten, FALSE if not
bool Cache::get_data(unordered_map<string, vector<Peak>> *& data)
{
	try
	{
		if (!check_cache())
			throw runtime_error("Cache is not valid to read data from!");
		// read data
		
		string fullpath;

		if (root.back() != '/')
			fullpath = root + "/" + MD5_string(filename) + "/" + CHR_SUBDIR;
		else
			fullpath = root + MD5_string(filename) + "/" + CHR_SUBDIR;

		WigCache * myFileCache = new WigCache(fullpath);

		if (myFileCache->deserialize(data))
			return true;
		else 
			throw runtime_error("Was not able to unserialize data!");
	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: get_data(): RUNTIME_ERROR:" << e.what() << endl;
		return false;
	}
	
	return false;	
}

/* 
 * Check if this file's cache is valid
 * Also creates new cache dir if one does not exist
 * returns TRUE if safe to read data, FALSE if not
 */
bool Cache::check_cache(void)
{
	try{	
		if (new_cache)
		{
			create_new_cache(); // creates cache director
			create_cache(); // creates file cache
			can_read_data = true;
		}
		else
		{
			// check MD5sums
			if (search_dir(MD5_string(filename)))
			{
				// directory for this file exists
				// check if file is same
				// get full path to the MD5sum file of directory
				string path_to_MD5sum = get_MD5sum_path(MD5_string(filename)); 
				string cache_MD5sum = retrieve_MD5sum(path_to_MD5sum);
				
				if (compare_file_MD5(cache_MD5sum, filename))
				{
					// cache is updated, all is well. Can read in data now
					can_read_data = true;
				}
				else
				{
					// MD5sums don't match: update cache
					update_cache();
					can_read_data = true;
				}
			}
			else 
			{
				create_cache();
				can_read_data = true;
			}
		}
		
	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: check_cache(): RUNTIME ERROR: " << e.what() << endl;
		return false;
	}

	return can_read_data;
}

/* returns TRUE if successful removal of file cache, FALSE if not */
bool Cache::rm_file_cache(string md5file)
{

	try
	{
		string md5path,fullpath, filepath, md5sum_filename;

		if (md5file != "")
			md5sum_filename = md5file;
		else
			md5sum_filename = MD5_string(filename);		
	
		if (root.back() != '/')
		{
			filepath = root + "/" + md5sum_filename + "/";
			md5path = root + "/" + md5sum_filename + "/" + MD5_FILENAME;
			fullpath = root + "/" + md5sum_filename + "/" + CHR_SUBDIR;
		}
		else
		{
			filepath = root + md5sum_filename + "/";
			md5path = root + md5sum_filename + "/" + MD5_FILENAME;
			fullpath = root + md5sum_filename + "/" + CHR_SUBDIR;
		}
		
		ifstream filetest(md5path.c_str());
	
		if (filetest)
		{
			filetest.close();
	
			if (!remove(md5path.c_str()))
				throw runtime_error("rm_file_cache: could not delete file [" + md5path + "]");				 
		}
		else
			throw runtime_error("rm_file_cache: could not open file [" + md5path + "]: file may not exist!");

		DIR * dirp;
		struct dirent * ds;
	
		if ((dirp = opendir(fullpath.c_str())) == NULL)
			throw runtime_error("rm_file_cache(): could not open DIR [" + fullpath + "]!");

		size_t found;

		while((ds = readdir(dirp)) != NULL)
			if ((found = string(ds->d_name).find(".dat")) != string::npos)
			{
				ifstream test(ds->d_name);

				if (!test)
				{
					closedir(dirp);
					throw runtime_error("rm_file_cache: could not open file [" + string(ds->d_name) + "]");
				}

				if(!remove(ds->d_name))
					throw runtime_error("rm_file_cache: could not delete file [" + string(ds->d_name) + "]");		 
			}
			else
			{
				closedir(dirp);
				throw runtime_error("Found a non- .dat file in directory [" + fullpath + "] : [" + string(ds->d_name) + "]! Remove this file before proceeding");
			}
			
		closedir(dirp);

		if (!rmdir(fullpath.c_str()))
			throw runtime_error("Could not remove directory [" + fullpath + "]");

		if (!rmdir(filepath.c_str()))
			throw runtime_error("Could not remove directory [" + filepath + "]");

		// placeholder
		return true;
	
	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: rm_file_cache(): RUNTIME_ERROR: " << e.what() << endl;
		return false;
	}

	return false;
}

/* returns TRUE if successful removal of cache, FALSE if not */
bool Cache::clear_cache(void)
{
	try
	{
		DIR * dirp;
		struct dirent * ds;
	
		if ((dirp = opendir(root.c_str())) == NULL)
			throw runtime_error("clear_cache(): could not open DIR [" + root + "]!");

		while((ds = readdir(dirp)) != NULL)
			if (string(ds->d_name) != "." && string(ds->d_name) != "..")
				if(!rm_file_cache(ds->d_name))
				{			
					closedir(dirp);
					throw runtime_error("clear_cache(): could not remove dir [" + string(ds->d_name) + "]");
				}			

		closedir(dirp);

		if (!rmdir(root.c_str()))
			throw runtime_error("clear_cache(): could not remove dir [" + root + "]");

		return true;

	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: clear_cache(): RUNTIME_ERROR: " << e.what() << endl;
		return false;
	}
	
	return false;
}

/* PRIVATE FUNCTIONS */
/* returns TRUE if successful creation of cache, FALSE if not */

void Cache::create_new_cache (void)
{
	cerr << "create_new_cache() start" << endl;
	int status = mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );

	if (status != 0)
		throw runtime_error("create_new_cache(): Directory creation of [" + root + "] was not successful!");	
	cerr << "create_new_cache() end" << endl;
	
	new_cache = false;
}

/* returns TRUE if successful creation of file cache, FALSE if not */
bool Cache::create_cache(void)
{
	cerr << "create_cache() start" << endl;
	string md5path,fullpath, filepath;

	if (root.back() != '/')
	{
		filepath = root + "/" + MD5_string(filename) + "/";
		md5path = root + "/" + MD5_string(filename) + "/" + MD5_FILENAME;
		fullpath = root + "/" + MD5_string(filename) + "/" + CHR_SUBDIR;
	}
	else
	{
		filepath = root + MD5_string(filename) + "/";
		md5path = root + MD5_string(filename) + "/" + MD5_FILENAME;
		fullpath = root + MD5_string(filename) + "/" + CHR_SUBDIR;
	}

	cerr << "create_cache() before mkdir" << endl;
	int status = mkdir(filepath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
	cerr << "create_cache() after mkdir" << endl;

	if (status != 0)
		throw runtime_error("create_cache(): Directory creation of [" + filepath + "] was not successful!");	

	ofstream md5file(md5path);
	
	if (!md5file.is_open())
		throw runtime_error("create_cache(): cannot open new md5 file path [" + md5path + "]");

	md5file << MD5_file(md5path);

	md5file.close();

	int chr_status = mkdir(fullpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
	if (chr_status != 0)
		throw runtime_error("create_cache(): Directory creation of [" + fullpath + "] was not successful!");	

	cerr << "create_cache() before new WigCache()" << endl;
	WigCache * myFileCache = new WigCache(fullpath);
	cerr << "create_cache() after new WigCache()" << endl;

	if (myFileCache->serialize(filename))
	{
		can_read_data = true;
		return true;
	}
	else
		throw runtime_error("create_cache(): Was not able to serialize data!");

	return false;	
}

/* returns TRUE if successful update of file cache, FALSE if not */
bool Cache::update_cache(void)
{

	// placeholder
	return true;
}

string Cache::get_MD5sum_path(string MD5str)
{
	string fullpath;

	if (root.back() != '/')
		fullpath = root + "/" + MD5str + "/" + MD5_FILENAME;
	else
		fullpath = root + MD5str + "/" + MD5_FILENAME;
				
	return fullpath;
}

string Cache::retrieve_MD5sum(string path)
{
	ifstream md5file(path);

	if (!md5file.is_open())
		throw runtime_error("retrieve_MD5sum(): Could not open md5file [" + path + "]!");	

	string line;

	if (!getline(md5file, line))
	{
		md5file.close();
		throw runtime_error("retrieve_MD5sum(): something is wrong with your md5 line! [" + line + "]");
	}

	md5file.close();

	return line;
}

bool Cache::search_dir(string md5)
{
	DIR * dirp;	
	struct dirent * ds;

	if ((dirp = opendir(root.c_str())) == NULL)
		throw runtime_error("search_dir(): DIR [" + root + "] cannot be opened!");

	while((ds = readdir(dirp)) != NULL)
		if (string(ds->d_name) == md5)
			return true;	

	return false;
}
