#ifndef TEST_H
#define TEST_H

#include"Utils.h"

class Test
{
	public:
	Test();

	bool test_all(void);
	bool test_md5(void);
	bool test_serialize(void);
	bool test_cache(void);
	
	private:
	void create_file(void);
	void modify_file(void);
	std::string file_cat(void);	
	std::string datastructure_to_string(std::unordered_map<std::string, std::vector<Peak>> *& data);

	std::string testwig;	
	std::string test_str;
};

#endif
