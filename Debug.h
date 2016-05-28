#ifndef DEBUG_H
#define DEBUG_H

#include<iostream>

// http://stackoverflow.com/questions/7880170/a-variable-that-is-read-only-after-assignment-at-run-time
int DebugLevel(int debug_init = 0); // this can only be initialized once

void debug (std::string err, int debug_lvl);

#endif
