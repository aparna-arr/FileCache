#include<iostream>
#include<cstring>
#include<fstream>
using namespace std;

#define CHROMSIZE 50
#define PEAKSIZE sizeof(Peak)

typedef struct {
	int start;	
	int end;
	char chrom[CHROMSIZE];
} Peak;
typedef struct {
	unsigned int start;	
	unsigned int end;
} SmallPeak;

typedef struct {
	unsigned int start;	
	unsigned int end;
	double value
} SmallPeakValue;

void writeBinary(const std::string & filename, Peak * data, int array_size)
{
	ofstream out;
	out.open(filename.c_str(), ios::binary);
	
	out.write(reinterpret_cast<char*>(&array_size), sizeof(int));
	for (int i = 0; i < array_size; i++)
		out.write(reinterpret_cast<char*>(&(data[i])), sizeof(Peak));

	out.close();
}

void writeTxt(const std::string & filename, Peak * data, int array_size)
{
	ofstream out;
	out.open(filename.c_str());
	
	out.write(reinterpret_cast<char*>(&array_size), sizeof(int));
	for (int i = 0; i < array_size; i++)
		out.write(reinterpret_cast<char*>(&(data[i])), sizeof(Peak));

	out.close();
}

int main()
{
	cout << "peak size is " << sizeof(Peak) << endl;
	cout << "small peak size is " << sizeof(SmallPeak) << endl;
	cout << "small value peak size is " << sizeof(SmallPeak) << endl;
	Peak peaks[2];
	
	peaks[0].start = 20;
	peaks[0].end = 40;
	strcpy(peaks[0].chrom, "chr2\0");
	
	peaks[1].start = 60;
	peaks[1].end = 80;
	strcpy(peaks[1].chrom, "chr4\0");

// these two output EXACTLY the same!
	writeBinary("test_binary.dat", peaks, 2);
	writeTxt("test_txt.txt", peaks, 2);
}
