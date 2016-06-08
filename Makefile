CXX := g++
CXXFLAGS := -Wall -std=c++11 -g
LDFLAGS := -lssl -lcrypto

SRC_COMMON := Cache.cpp Debug.cpp MD5sum.cpp Serialize.cpp Utils.cpp
SRC_TEST := TestDriver.cpp Test.cpp $(SRC_COMMON)
SRC_PRG := Main.cpp UserInput.cpp $(SRC_COMMON)

OBJ_TEST := $(SRC_TEST:.cpp=.o)
OBJ_PRG := $(SRC_PRG:.cpp=.o)

EXEC_PRG := cache
EXEC_TEST := test-cache

all: $(EXEC_PRG) $(EXEC_TEST)

$(EXEC_PRG): $(OBJ_PRG)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(EXEC_TEST): $(OBJ_TEST)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean: 
	rm -f $(OBJ_TEST) $(OBJ_PRG) $(EXEC_PRG) $(EXEC_TEST)
