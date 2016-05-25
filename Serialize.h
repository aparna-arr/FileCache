#ifndef SERIALIZE_H
#define SERIALIZE_H

#include<iostream>
#include<stdexcept>
#include<unordered_map>
#include<vector>

#define CHROMSIZE 50 // max size of chromosome name in characters

typedef struct {
	unsigned int start;
	unsigned int end;
	double value;
} Peak;

class WigCache 
{
	public:
	WigCache(std::string fullpath);
	
	bool serialize(std::string file);
	bool deserialize(std::unordered_map<std::string, std::vector<Peak>> *& data);

	private:
	bool readInWig(std::string filename, std::unordered_map<std::string, std::vector<Peak>> *& peaks);
	bool vectorToArray(std::vector<Peak> *& peaks_vec, Peak *& peaks_ar); 
	bool flatten(Peak * peaks_ar, int numPeaks, std::string file);
	bool unflatten(Peak *& peaks_ar, int & numPeaks, std::string file);
	
	std::string path;
};



#endif
