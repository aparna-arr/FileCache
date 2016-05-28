#ifndef CACHE_H
#define CACHE_H

#include<iostream>
#include<stdexcept>
#include"MD5sum.h"
#include"Serialize.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fstream>
#include<unordered_map>
#include<vector>
#include"dirent.h"
#include<cstdio>
#include"Debug.h"

#define MD5_FILENAME "md5sum.txt"
#define CHR_SUBDIR "chrs/"

class Cache
{
	public:
	Cache(void); // you better know what you're doing if you use this: only included for cases where you want to make an array of new Cache objs ptrs to init later or something
	Cache(std::string cache_root, std::string file);

	bool get_data(std::unordered_map<std::string, std::vector<Peak>> *& data); // runs check_cache()	
	
	bool check_cache(void); // checks if previous cache is valid for a single file, if not creates or rebuilds
	bool rm_file_cache(std::string md5file = ""); // deletes a single file from cache 
	bool clear_cache(void); // removes entire cache

	private:
	void create_new_cache(void);
	bool create_cache(void); // user not allowed to directly call this: use check_cache(), creates new file cache for single file
	bool update_cache(void); // removes previous cache for a single file, rebuilds

	std::string get_MD5sum_path(std::string MD5str);
	std::string retrieve_MD5sum(std::string path);
	bool search_dir(std::string md5);

	std::string root, filename;
	bool new_cache, can_read_data;
};

#endif
