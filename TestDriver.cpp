#include"Utils.h"
#include"Test.h"

using namespace std;

int main(int argc, char * argv[])
{
	Test * testCache = new Test();

	if (!testCache->test_all())
		cout << endl << RED << "***TESTS FAILED***" << DEFAULT << endl;
	else
		cout << endl << RED << "***TESTS PASSED***" << DEFAULT << endl;
	
}
