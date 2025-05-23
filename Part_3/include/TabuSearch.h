#ifndef TABU_SEARCH_H_
#define TABU_SEARCH_H_

#include <vector>
#include <string>
#include "Element.h"

class TabuSearch
{
private:
    std::vector<Element> elements;
    int capacity;
    int tabu_duration;
    int current_iteration;
    std::vector<bool> best_solution;
    std::vector<int> best_solution_details; // value, weight, iteration
    std::vector<bool> best_solution_ts; // tabu list

public:
    TabuSearch(const std::string &filename, int td);

    void run(int iterations, int output_interval);
    bool add();
    void clear();

    void loadData(const std::string &filename);
    void updateBestSolution();

    void printHeader() const;
    void printSolution() const;
    void printBestSolution() const;

    bool isTabu(int index) const;
    bool canFit(int index) const;

    int getCurrentWeight() const;
    int getCurrentValue() const;
    std::vector<bool> getCurrentSolution() const;
    std::vector<bool> getTabuList() const;
    std::string strVector(const std::vector<bool> &state) const;
};

#endif // TABU_SEARCH_H_