#ifndef UTILS_H
#define UTILS_H

#include<iostream>
#include<stdexcept>
#include<unistd.h>
#include<sstream>
#include<vector>
#include"Debug.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define DEFAULT "\033[0m"


std::string conv(int num);
std::string conv(double num);

bool toInt(std::string str, int & num);

#endif
