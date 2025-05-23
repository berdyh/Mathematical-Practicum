# -------------------------------------------------------------------
# Makefile: builds `./tabu`
# -------------------------------------------------------------------

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude


.PHONY: all tabu clean

all: tabu

# main test driver
tabu:
	$(CXX) $(CXXFLAGS) src/TabuSearch.cpp src/main.cpp -o tabu

clean:
	rm -f tabu

