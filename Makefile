CXX := g++
CXXFLAGS := -Wall -std=c++11 -g
OBJECTS := Main.o Utils.o Cache.o MD5sum.o Serialize.o

cache: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ -lssl -lcrypto

Main.o: Main.cpp Utils.h Cache.h 
	$(CXX) $(CXXFLAGS) -c Main.cpp 

Utils.o: Utils.cpp Utils.h
	$(CXX) $(CXXFLAGS) -c Utils.cpp 

Cache.o: Cache.h MD5sum.h Serialize.h
	$(CXX) $(CXXFLAGS) -c Cache.cpp

MD5sum.o: MD5sum.cpp MD5sum.h
	$(CXX) $(CXXFLAGS) -c MD5sum.cpp -lssl -lcrypto

Serialize.o: Serialize.cpp Serialize.h
	$(CXX) $(CXXFLAGS) -c Serialize.cpp

clean: 
	rm -f $(OBJECTS) cache
