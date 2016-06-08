#ifndef USER_INPUT_H
#define USER_INPUT_H

#include"Utils.h"
void usage(void);
void handle_opts(int argc, char * argv[], std::string &cache_root, std::string &filename, bool & remove, bool & remove_all);

#endif
