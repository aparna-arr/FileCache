#include"Cache.h"
using namespace std;

// later functions DO NOT CHECK the validity of the root or filename afterconstructor: USE THIS ONLY IF YOU KNOW WHAT YOU ARE DOING
Cache::Cache(void)
{
	debug("Cache::Cache(): begin", 1);

	root = "INIT";
	filename = "INIT";
	new_cache = false;
	can_read_data = false;

	debug("Cache::Cache(): end", 1);
}

Cache::Cache(string cache_root)
{
	try
	{
		debug("Cache::Cache(string): begin", 1);

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
//			cerr << "Cache does not exist: will create new cache\n";
			new_cache = true;
			root = cache_root;
		}
		
		debug("Cache::Cache(string): end", 1);
	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: Cache(): RUNTIME_ERROR: " << e.what() << endl;
	}
}


Cache::Cache(string cache_root, string file)
{
	try
	{
		debug("Cache::Cache(string, string): begin", 1);

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
//			cerr << "Cache does not exist: will create new cache\n";
			new_cache = true;
			root = cache_root;
		}
		
		ifstream test(file.c_str());

		if (test)
			filename = file;
		else
			throw runtime_error("Cannot read file [" + file + "]: File does not exist or is unreadable!");
		
		test.close();

		debug("Cache::Cache(string, string): end", 1);
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
		debug("Cache::get_data(): begin", 1);

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
		{
			debug("Cache::get_data(): end", 1);

			return true;
		}
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
		debug("Cache::check_cache(): begin", 1);
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
		debug("Cache::check_cache(): end", 1);
		return can_read_data;
	}
	catch (const runtime_error &e)
	{
		cerr << "CACHE: check_cache(): RUNTIME ERROR: " << e.what() << endl;
		return false;
	}

}

/* returns TRUE if successful removal of file cache, FALSE if not */
bool Cache::rm_file_cache(string md5file)
{
	try
	{
		debug("Cache::rm_file_cache(): begin", 1);

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

		DIR * testdir;
		if ((testdir = opendir(filepath.c_str())) == NULL)
			throw runtime_error("rm_file_cache(): could not open DIR [" + filepath + "]!");

		closedir(testdir);
		
		ifstream filetest(md5path.c_str());
	
		if (filetest)
		{
	
			filetest.close();
			if (remove(md5path.c_str()) != 0)
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
		{
			string curr_file = fullpath + string(ds->d_name);
			if ((found = string(ds->d_name).find(".dat")) != string::npos)
			{
//				cerr << "rm_file_cache(): on file [" << string(ds->d_name) << "] WHICH HAS A .dat!" << endl;
				ifstream test(curr_file, ios::binary);

				if (!test)
				{
					closedir(dirp);
					throw runtime_error("rm_file_cache: could not open .dat file [" + curr_file + "]");
				}

				test.close();
				if(remove(curr_file.c_str()) != 0)
				{
					closedir(dirp);
					throw runtime_error("rm_file_cache: could not delete .dat file [" + curr_file + "]");		
				} 
			}
			else if (string(ds->d_name) != "." && string(ds->d_name) != ".." && string(ds->d_name) != "")
			{
				closedir(dirp);
				throw runtime_error("Found a non- .dat file in directory [" + fullpath + "] : [" + curr_file + "]! Remove this file before proceeding");
			}
		}
			
		closedir(dirp);

		if (rmdir(fullpath.c_str()) != 0)
			throw runtime_error("Could not remove directory [" + fullpath + "]");

		if (rmdir(filepath.c_str()) != 0)
			throw runtime_error("Could not remove directory [" + filepath + "]");

		debug("Cache::rm_file_cache(): end", 1);
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
		debug("Cache::clear_cache(): begin", 1);
		DIR * dirp;
		struct dirent * ds;
	
		if ((dirp = opendir(root.c_str())) == NULL)
			throw runtime_error("clear_cache(): could not open DIR [" + root + "]!");

		while((ds = readdir(dirp)) != NULL)
			if (string(ds->d_name) != "." && string(ds->d_name) != "..")
			{
/*
				string curr_dir;

				if (root.back() != '/')
					curr_dir = root + "/" + string(ds->d_name);
				else
					curr_dir = root + string(ds->d_name);
*/
				string curr_dir = string(ds->d_name);

//				cerr << "clear_cache(): curr_dir is " << curr_dir << endl;

				if(!rm_file_cache(curr_dir))
				{			
					closedir(dirp);
					throw runtime_error("clear_cache(): could not remove dir [" + curr_dir + "]");
				}	
			}		

		closedir(dirp);

		if (rmdir(root.c_str()) != 0)
			throw runtime_error("clear_cache(): could not remove dir [" + root + "]");

		debug("Cache::clear_cache(): end", 1);
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
	debug("Cache::create_new_cache(): begin", 1);
	int status = mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );

	if (status != 0)
		throw runtime_error("create_new_cache(): Directory creation of [" + root + "] was not successful!");	
//	cerr << "create_new_cache() end" << endl;
	
	new_cache = false;
	debug("Cache::create_new_cache(): end", 1);
}

/* returns TRUE if successful creation of file cache, FALSE if not */
bool Cache::create_cache(void)
{
	debug("Cache::create_cache(): begin", 1);
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

//	cerr << "create_cache() before mkdir" << endl;
	int status = mkdir(filepath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
//	cerr << "create_cache() after mkdir" << endl;

	if (status != 0)
		throw runtime_error("create_cache(): Directory creation of [" + filepath + "] was not successful!");	

	ofstream md5file(md5path);
	
	if (!md5file.is_open())
		throw runtime_error("create_cache(): cannot open new md5 file path [" + md5path + "]");

//	cerr << "create_cache(): MD5_file is [" << MD5_file(filename) << "] for [" << filename << "]" << endl;

	md5file << MD5_file(filename);

	md5file.close();

	int chr_status = mkdir(fullpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
	if (chr_status != 0)
		throw runtime_error("create_cache(): Directory creation of [" + fullpath + "] was not successful!");	

//	cerr << "create_cache() before new WigCache()" << endl;
	WigCache * myFileCache = new WigCache(fullpath);
//	cerr << "create_cache() after new WigCache()" << endl;

	if (myFileCache->serialize(filename))
	{
		can_read_data = true;
		debug("Cache::create_cache(): end", 1);
		return true;
	}
	else
		throw runtime_error("create_cache(): Was not able to serialize data!");

	return false;	
}

/* returns TRUE if successful update of file cache, FALSE if not */
bool Cache::update_cache(void)
{
	debug("Cache::update_cache(): begin", 1);
	if (!rm_file_cache())
		throw runtime_error("update_cache(): could not remove depreciated file cache");

	if (!create_cache())
		throw runtime_error("update_cache(): could not create updated file cache");
	
	debug("Cache::update_cache(): end", 1);
	return true;
}

string Cache::get_MD5sum_path(string MD5str)
{
	debug("Cache::get_MD5sum_path(): begin", 1);
	string fullpath;

	if (root.back() != '/')
		fullpath = root + "/" + MD5str + "/" + MD5_FILENAME;
	else
		fullpath = root + MD5str + "/" + MD5_FILENAME;

	debug("Cache::get_MD5sum_path(): end", 1);
				
	return fullpath;
}

string Cache::retrieve_MD5sum(string path)
{
	debug("Cache::retrieve_MD5sum(): begin", 1);
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

	debug("Cache::retrieve_MD5sum(): end", 1);

	return line;
}

bool Cache::search_dir(string md5)
{
	debug("Cache::search_dir(): begin", 1);
	DIR * dirp;	
	struct dirent * ds;

	if ((dirp = opendir(root.c_str())) == NULL)
		throw runtime_error("search_dir(): DIR [" + root + "] cannot be opened!");

	while((ds = readdir(dirp)) != NULL)
		if (string(ds->d_name) == md5)
		{
			debug("Cache::search_dir(): end", 1);
			return true;	
		}

	debug("Cache::search_dir(): end", 1);
	return false;
}
