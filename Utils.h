#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<stdexcept>
#include<unistd.h>
#include<sstream>
#include"Cache.h"

void usage(void);
void handle_opts(int argc, char * argv[], std::string &cache_root, std::string &filename);
#endif
