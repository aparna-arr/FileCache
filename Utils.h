#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<stdexcept>
#include<unistd.h>
#include<sstream>
#include"Debug.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define DEFAULT "\033[0m"

void usage(void);

std::string conv(int num);
std::string conv(double num);

#endif
