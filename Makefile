CXX := g++
CXXFLAGS := -Wall -std=c++11 -g
OBJECTS := UserIn.o Utils.o Cache.o MD5sum.o Serialize.o Debug.o Test.o

all: cache test-cache

cache: Main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) Main.o $(OBJECTS) -o $@ -lssl -lcrypto

test-cache: TestDriver.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) TestDriver.o $(OBJECTS) -o $@ -lssl -lcrypto

Main.o: UserIn.h Cache.h
	$(CXX) $(CXXFLAGS) -c Main.cpp 

TestDriver.o: Test.h 
	$(CXX) $(CXXFLAGS) -c TestDriver.cpp 

Test.o: Test.h
	$(CXX) $(CXXFLAGS) -c Test.cpp 

UserIn.o: UserIn.h
	$(CXX) $(CXXFLAGS) -c UserIn.cpp 

Utils.o: Utils.h
	$(CXX) $(CXXFLAGS) -c Utils.cpp 

Cache.o: Cache.h
	$(CXX) $(CXXFLAGS) -c Cache.cpp

MD5sum.o: MD5sum.h 
	$(CXX) $(CXXFLAGS) -c MD5sum.cpp -lssl -lcrypto

Serialize.o: Serialize.h 
	$(CXX) $(CXXFLAGS) -c Serialize.cpp

Debug.o: Debug.h
	$(CXX) $(CXXFLAGS) -c Debug.cpp

clean: 
	rm -f $(OBJECTS) Main.o TestDriver.o cache test-cache
