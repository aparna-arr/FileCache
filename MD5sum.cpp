#include"MD5sum.h"
using namespace std;

std::string MD5_string(std::string str)
{
	MD5_CTX mdContext;
	unsigned char c[MD5_DIGEST_LENGTH];

	MD5_Init(&mdContext);
	MD5_Update(&mdContext, (unsigned char *)str.c_str(), str.length());
	MD5_Final(c, &mdContext);
	
	char c_result[MD5_DIGEST_LENGTH*2+1];
	
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&c_result[i*2], "%02x", c[i]);
		
	return string(c_result);
}

string MD5_file(string filename)
{
	// find md5 of file
	// efficient alg for large files
	// source: http://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c

	unsigned char c[MD5_DIGEST_LENGTH];
	
	FILE * fp = fopen(filename.c_str(), "rb");
	MD5_CTX mdContext;
	unsigned char data[1024];

	if (fp == NULL) 
		throw runtime_error("compare_file_MD5(): Can't open file [" + filename + "]");

	MD5_Init(&mdContext);
	int bytes;

	while ((bytes = fread(data,1,1024,fp)) != 0)
		MD5_Update(&mdContext, data, bytes);
	
	MD5_Final(c, &mdContext);
	
	char c_result[MD5_DIGEST_LENGTH*2+1];

	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&c_result[i*2], "%02x", c[i]);

	fclose(fp);		

	return string(c_result);
}

bool compare_file_MD5(std::string md5_str, std::string filename)
{
	cerr << "MD5 of file is [" << MD5_file(filename) << "]" << endl;
	cerr << "MD5 of str is  [" << md5_str << "]" << endl;
	if (MD5_file(filename) == md5_str)
		return true;

	return false;
}
