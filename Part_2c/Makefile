# -------------------------------------------------------------------
# Makefile: builds `./sort` and `./test` using test/*.txt
# -------------------------------------------------------------------

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

SRC_GLOB := src/*.cpp
UNIT_OBJ  := include/unit.o

.PHONY: all sort test clean

all: sort test

# main test driver
sort:
	$(CXX) $(CXXFLAGS) $(UNIT_OBJ) $(SRC_GLOB) test_files/main.cpp -o sort

# Secondary test driver
test:
	$(CXX) $(CXXFLAGS) $(UNIT_OBJ) $(SRC_GLOB) test_files/test.cpp -o test

clean:
	rm -f sort test

