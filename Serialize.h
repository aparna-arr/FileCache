#ifndef SERIALIZE_H
#define SERIALIZE_H

#include<iostream>
#include<stdexcept>
#include<unordered_map>
#include<vector>
#include<fstream>
#include<sstream>
#include"dirent.h"
#include"Debug.h"
#include"Utils.h"

#define CHROMSIZE 50 // max size of chromosome name in characters

typedef struct {
	unsigned int start;
	unsigned int end;
	double value;
} Peak;

typedef struct {
	unsigned int numPeaks;
	Peak * peaks;
} PeakInfo;

class WigCache 
{
	public:
	WigCache(std::string fullpath);
	
	bool serialize(std::string file);
	bool deserialize(std::unordered_map<std::string, PeakInfo> *& data);

	private:
	bool readInWig(std::string filename, std::unordered_map<std::string, PeakInfo> *& peaks);
//	bool vectorToArray(std::vector<Peak> & peaks_vec, Peak *& peaks_ar); 
//	bool arrayToVector(Peak * peaks_ar, int numPeaks, std::vector<Peak> & peaks_vec); 
	bool flatten(Peak * peaks_ar, unsigned int numPeaks, std::string file);
	bool unflatten(Peak *& peaks_ar, unsigned int & numPeaks, std::string file);
	
	std::string path;
};



#endif
