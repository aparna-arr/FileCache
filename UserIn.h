#ifndef USERIN_H
#define USERIN_H

#include"unistd.h"

#include "Utils.h"

/*
#include"Debug.h"
#include<stdexcept>
#include<sstream>
void usage();
*/
void handle_opts(int argc, char * argv[], std::string &cache_root, std::string &filename, bool & remove, bool & remove_all);

#endif
