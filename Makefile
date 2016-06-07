CXX := g++
CXXFLAGS := -Wall -std=c++11 -g
OBJECTS := Utils.o Cache.o MD5sum.o Serialize.o Debug.o Test.o

all: cache test-cache

cache: Main.o UserInput.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) Main.o UserInput.o $(OBJECTS) -o $@ -lssl -lcrypto

test-cache: TestDriver.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) TestDriver.o $(OBJECTS) -o $@ -lssl -lcrypto

Main.o: UserInput.h Cache.h Utils.h 
	$(CXX) $(CXXFLAGS) -c Main.cpp 

TestDriver.o: Utils.h Test.h
	$(CXX) $(CXXFLAGS) -c TestDriver.cpp 

Test.o: Test.h
	$(CXX) $(CXXFLAGS) -c Test.cpp 

Utils.o: Utils.h
	$(CXX) $(CXXFLAGS) -c Utils.cpp 

UserInput.o: UserInput.h
	$(CXX) $(CXXFLAGS) -c UserInput.cpp 

Cache.o: Cache.h
	$(CXX) $(CXXFLAGS) -c Cache.cpp

MD5sum.o: MD5sum.h 
	$(CXX) $(CXXFLAGS) -c MD5sum.cpp -lssl -lcrypto

Serialize.o: Serialize.h 
	$(CXX) $(CXXFLAGS) -c Serialize.cpp

Debug.o: Debug.h
	$(CXX) $(CXXFLAGS) -c Debug.cpp

clean: 
	rm -f $(OBJECTS) Main.o TestDriver.o UserInput.o cache test-cache
