#include"MD5sum.h"
using namespace std;

std::string MD5_string(std::string str)
{

	// placeholder
	return "PLACEHOLDER";
}

bool compare_file_MD5(std::string md5_str, std::string filename)
{
	// find md5 of file
	// efficient alg for large files
	// source: http://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c

	unsigned char c[MD5_DIGEST_LENGTH];
	
	FILE * fp = fopen(filename.c_str(), "rb");
	MD5_CTX mdContext;
	unsigned char data[1024];

	if (fp == NULL) 
	{
		cerr << "Can't open file [" << filename << "]" << endl;
		return false;
	}	

	MD5_Init(&mdContext);
	int bytes;
	while ((bytes = fread(data,1,1024,fp)) != 0)
		MD5_Update(&mdContext, data, bytes);
	
	MD5_Final(c, &mdContext);
	
	char c_result[MD5_DIGEST_LENGTH];

	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(c_result, "%02x", c[i]);

	fclose(fp);		

	if (string(c_result) == md5_str)
		return true;

	return false;
}
