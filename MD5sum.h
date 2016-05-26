#ifndef MD5SUM_H
#define MD5SUM_H
#include<iostream>
#include<openssl/md5.h>
#include<stdexcept>
#include<cstring>

std::string MD5_string(std::string str);
std::string MD5_file(std::string filename);
bool compare_file_MD5(std::string md5_str, std::string filename);

#endif
