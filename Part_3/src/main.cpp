#include <iostream>
#include "../include/TabuSearch.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: ./tabu <datafile> <tabu_duration> <iterations> <output_interval>" << std::endl;
        return 1;
    }

    std::string datafile = argv[1];
    int tabu_duration = std::stoi(argv[2]);
    int iterations = std::stoi(argv[3]);
    int output_interval = std::stoi(argv[4]);

    TabuSearch ts(datafile, tabu_duration);
    std::cout << "Debug: " << ts.strVector(ts.getTabuList()) << "\n"
              << "Debug: " << ts.strVector(ts.getCurrentSolution()) << std::endl;
    ts.run(iterations, output_interval);

    return 0;
}