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

string Cache::get_MD5sum_path(string MD5str)
{
	string fullpath;

	if (root.back() != '/')
		fullpath += "/" + MD5str + "/" + MD5_FILENAME;
	else
		fullpath += MD5str + "/" + MD5_FILENAME;
				
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
