#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<stdexcept>
#include<unistd.h>
#include<sstream>
#include"Cache.h"
#include"Debug.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define DEFAULT "\033[0m"

void usage(void);
void handle_opts(int argc, char * argv[], std::string &cache_root, std::string &filename, bool & remove, bool & remove_all);
#endif
