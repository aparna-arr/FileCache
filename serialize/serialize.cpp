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

void printPeak(Peak * peak);
void printPeak(Peak peak);

void write(const std::string & filename, Peak & data)
{
	ofstream out;
	out.open(filename.c_str(), ios::binary);
	out.write(reinterpret_cast<char*>(&data), sizeof(Peak));
	out.close();
}

void read(const string & filename, Peak & data)
{
	ifstream in;
	in.open(filename.c_str(), ios::binary);
	in.read(reinterpret_cast<char*>(&data), sizeof(Peak));
	in.close();
}

void writeMultiple(const std::string & filename, Peak * data, int array_size)
{
	ofstream out;
	out.open(filename.c_str(), ios::binary);

	for (int i = 0; i < array_size; i++)
		out.write(reinterpret_cast<char*>(&(data[i])), sizeof(Peak));

	out.close();
}

void readMultiple(const string & filename, Peak * data, int array_size)
{
	ifstream in;
	in.open(filename.c_str(), ios::binary);
	
	for (int i = 0; i < array_size; i++)
		in.read(reinterpret_cast<char*>(&(data[i])), sizeof(Peak));

	in.close();
}

void writeMultipleUnknownSizeRead(const std::string & filename, Peak * data, int array_size)
{
	ofstream out;
	out.open(filename.c_str(), ios::binary);
	
//	out << array_size << endl;
	out.write(reinterpret_cast<char*>(&array_size), sizeof(int));
	for (int i = 0; i < array_size; i++)
		out.write(reinterpret_cast<char*>(&(data[i])), sizeof(Peak));

	out.close();
}

void readMultipleUnknownSize(const string & filename, Peak * data)
{
	ifstream in;
	in.open(filename.c_str(), ios::binary);
	
	int size;

	in.read(reinterpret_cast<char*>(&size), sizeof(int));	
	for (int i = 0; i < size; i++)
		in.read(reinterpret_cast<char*>(&(data[i])), sizeof(Peak));

	in.close();
}

int main()
{
	Peak *mypeak = new Peak;
		
	mypeak->start = 10;
	mypeak->end = 20;
	strcpy(mypeak->chrom, "chr1\0") ;

	printPeak(mypeak);

	write("test.dat", *mypeak);

	Peak * newpeak = new Peak;

	read("test.dat", *newpeak);

	printPeak(newpeak);

	cout << "MULTIPLE" << endl;
	cout << "WRITE" << endl;
	
	Peak peaks[2], outpeaks[2];
	
	peaks[0].start = 20;
	peaks[0].end = 40;
	strcpy(peaks[0].chrom, "chr2\0");
	
	peaks[1].start = 60;
	peaks[1].end = 80;
	strcpy(peaks[1].chrom, "chr4\0");

	printPeak(peaks[0]);
	printPeak(peaks[1]);

	writeMultiple("test_mult.dat", peaks, 2);
	cout << "READ" << endl;
	
	readMultiple("test_mult.dat", outpeaks, 2);
	
	printPeak(outpeaks[0]);
	printPeak(outpeaks[1]);

	cout << "MULTIPLE UNKNOWN SIZE" << endl;
	cout << "WRITE" << endl;
	
	Peak peaks2[2], outpeaks2[2];
	
	peaks2[0].start = 20;
	peaks2[0].end = 40;
	strcpy(peaks2[0].chrom, "chr2\0");
	
	peaks2[1].start = 60;
	peaks2[1].end = 80;
	strcpy(peaks2[1].chrom, "chr4\0");

	printPeak(peaks2[0]);
	printPeak(peaks2[1]);

	writeMultipleUnknownSizeRead("test_mult.dat", peaks2, 2);
	cout << "READ" << endl;
	
	readMultipleUnknownSize("test_mult.dat", outpeaks2);
	
	printPeak(outpeaks2[0]);
	printPeak(outpeaks2[1]);

}
void printPeak(Peak peak)
{
	cout << "Peak start: [" << peak.start << "]" << endl;
	cout << "Peak end: [" << peak.end << "]" << endl;
	cout << "Peak chr: [" << peak.chrom << "]" << endl;
}

void printPeak(Peak * peak)
{
	cout << "Peak start: [" << peak -> start << "]" << endl;
	cout << "Peak end: [" << peak -> end << "]" << endl;
	cout << "Peak chr: [" << peak -> chrom << "]" << endl;
}
