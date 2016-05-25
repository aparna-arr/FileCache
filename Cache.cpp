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
		cerr << "CACHE: RUNTIME_ERROR: " << e.what() << endl;
	}
}

unordered_map<string, vector<Peak>> * Cache::get_data(void)
{
	if (check_cache())
	{
		cerr << "Cache is not valid to read data from!" << endl;
		return NULL;
	} 

	// read data
	
	// placeholder
	return NULL;	
}

/* 
 * Check if this file's cache is valid
 * Also creates new cache dir if one does not exist
 */
bool Cache::check_cache()
{
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
			string path_to_MD5sum = get_MD5sum_path(); 
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
	
	return can_read_data;
}

bool Cache::rm_file_cache(void)
{

	// placeholder
	return true;
}

bool Cache::clear_cache(void)
{

	// placeholder
	return true;
}

/* PRIVATE FUNCTIONS */

void Cache::create_new_cache (void)
{

}

bool Cache::create_cache(void)
{

	// placeholder
	return true;
}

bool Cache::update_cache(void)
{

	// placeholder
	return true;
}

string Cache::get_MD5sum_path(void)
{

	// placeholder
	return "PLACEHOLDER";
}

string Cache::retrieve_MD5sum(string path)
{

	// placeholder
	return "PLACEHOLDER";
}

bool Cache::search_dir(string md5)
{
	
	// placeholde
	return true;
}
