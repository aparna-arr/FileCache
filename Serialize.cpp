#include"Serialize.h"

WigCache::WigCache(std::string fullpath)
{

}

bool WigCache::serialize(std::string file)
{

	// placeholder
	return true;
}

bool WigCache::deserialize(std::unordered_map<std::string, std::vector<Peak>> *& data)
{

	// placeholder
	return true;
}

bool WigCache::readInWig(std::string filename, std::unordered_map<std::string, std::vector<Peak>> *& peaks)
{

	// placeholder
	return true;
}

bool WigCache::vectorToArray(std::vector<Peak> *& peaks_vec, Peak *& peaks_ar) 
{

	// placeholder
	return true;
}

bool WigCache::flatten(Peak * peaks_ar, int numPeaks, std::string file)
{

	// placeholder
	return true;
}

bool WigCache::unflatten(Peak *& peaks_ar, int & numPeaks, std::string file)
{

	// placeholder
	return true;
}
