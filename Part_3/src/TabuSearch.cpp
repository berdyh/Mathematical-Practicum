#include "../include/TabuSearch.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>

TabuSearch::TabuSearch(const std::string &filename, int td)
    : tabu_duration(td), current_iteration(0), best_solution_details({0, 0, 0})
{
    loadData(filename);

    // Initialize the best solution with the first element
    best_solution.resize(elements.size(), false);

    while (add())
    {
    }
    for (size_t i = 0; i < elements.size(); i++)
    {
        elements[i].touched = -1; // reset touched for all elements
    }

    // Update the best solution
    updateBestSolution();
}

void TabuSearch::loadData(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    int num_elements;
    file >> num_elements >> capacity;

    elements.resize(num_elements);
    for (int i = 0; i < num_elements; i++)
    {
        file >> elements[i].money >> elements[i].weight;
        elements[i].touched = -1; // not moved yet
        elements[i].in_x = false; // not in the bag
    }
    file.close();
}

void TabuSearch::run(int iterations, int output_interval)
{
    std::cout << "Capacity: " << capacity << "\n"
              << "Number of elements: " << elements.size() << "\n"
              << "Tabu duration: " << tabu_duration << "\n"
              << std::endl;

    if (output_interval > 0)
    {
        std::cout << "Stepping through iterations..." << std::endl;
        printHeader();
        updateBestSolution();
        printSolution();
    }

    current_iteration = 0;
    while (current_iteration < iterations - 1)
    {

        if (!add())
        {
            clear();
            current_iteration++;
        }
        else
        {
            current_iteration++;
        }

        updateBestSolution();
        if (output_interval > 0 && (current_iteration + 1) % output_interval == 0)
        {
            printSolution();
        }
    }

    std::cout << "Final best solution: " << std::endl;
    printHeader();
    printBestSolution();
}

bool TabuSearch::add()
{
    int best_item = -1;
    int best_gain = 0;

    // Find the best item to add
    for (size_t i = 0; i < elements.size(); i++)
    {
        if (!elements[i].in_x && !isTabu(i) && canFit(i))
        {
            if (elements[i].money > best_gain || (elements[i].money == best_gain && best_item != -1 && elements[i].weight < elements[best_item].weight))
            {
                best_gain = elements[i].money;
                best_item = i;
            }
        }
    }

    if (best_item != -1)
    {
        elements[best_item].in_x = true;
        elements[best_item].touched = current_iteration;
        return true;
    }
    return false;
}

void TabuSearch::clear()
{
    int worst_item = -1;
    int worst_value = INT_MAX;

    // Find the worst item to remove
    for (size_t i = 0; i < elements.size(); i++)
    {
        if (elements[i].in_x && !isTabu(i))
        {
            if (elements[i].money < worst_value || (worst_value == elements[i].money && worst_item != -1 && elements[i].weight > elements[worst_item].weight))
            {
                worst_value = elements[i].money;
                worst_item = i;
            }
        }
    }

    if (worst_item != -1)
    {
        elements[worst_item].in_x = false;
        elements[worst_item].touched = current_iteration;
    }
}

void TabuSearch::updateBestSolution()
{
    int current_gain = getCurrentValue();
    if (current_gain > best_solution_details[0])
    {
        best_solution_details[0] = current_gain;
        best_solution_details[1] = getCurrentWeight();
        best_solution_details[2] = current_iteration;
        best_solution_ts = getTabuList();

        // Update the best solution
        for (size_t i = 0; i < elements.size(); i++)
        {
            best_solution[i] = elements[i].in_x;
        }
    }
}

void TabuSearch::printSolution() const
{
    std::cout << std::left << std::setw(6) << current_iteration
              << std::setw(7) << getCurrentValue()
              << std::setw(8) << getCurrentWeight()
              << std::setw(20) << strVector(getCurrentSolution())
              << std::setw(25) << strVector(getTabuList())
              << std::endl;
    std::cout << std::string(66, '-') << std::endl;
}

void TabuSearch::printBestSolution() const
{
    std::cout << std::left << std::setw(6) << best_solution_details[2]
              << std::setw(7) << best_solution_details[0]
              << std::setw(8) << best_solution_details[1]
              << std::setw(20) << strVector(best_solution)
              << std::setw(25) << strVector(best_solution_ts)
              << std::endl;
}

void TabuSearch::printHeader() const
{
    std::cout << std::left << std::setw(6) << "Iter"
              << std::setw(7) << "Gain"
              << std::setw(8) << "Weight"
              << std::setw(20) << "InBag"
              << std::setw(25) << "Tabu"
              << std::endl;
    std::cout << std::string(66, '-') << std::endl;
}

// Helper functions to check if an item is tabu or can fit in the bag
bool TabuSearch::isTabu(int item_index) const
{
    return elements[item_index].touched >= 0 &&
           current_iteration <= elements[item_index].touched + tabu_duration;
}

bool TabuSearch::canFit(int item_index) const
{
    int current_weight = getCurrentWeight();
    return current_weight + elements[item_index].weight <= capacity;
}

int TabuSearch::getCurrentWeight() const
{
    int weight = 0;
    for (const auto &element : elements)
    {
        if (element.in_x)
        {
            weight += element.weight;
        }
    }
    return weight;
}

int TabuSearch::getCurrentValue() const
{
    int value = 0;
    for (const auto &element : elements)
    {
        if (element.in_x)
        {
            value += element.money;
        }
    }
    return value;
}

std::vector<bool> TabuSearch::getCurrentSolution() const
{
    std::vector<bool> current(elements.size(), false);
    for (size_t i = 0; i < elements.size(); ++i)
        current[i] = elements[i].in_x;
    return current;
}

std::vector<bool> TabuSearch::getTabuList() const
{
    std::vector<bool> tabu_list(elements.size(), false);
    for (size_t i = 0; i < elements.size(); i++)
    {
        if (isTabu(i))
        {
            tabu_list[i] = true;
        }
    }

    return tabu_list;
}

std::string TabuSearch::strVector(const std::vector<bool> &state) const
{
    bool first = true;
    std::string result = "[";
    for (size_t i = 0; i < state.size(); ++i)
    {
        if (state[i])
        {
            if (first)
            {
                result += std::to_string(i + 1);
                first = false;
            }
            else
            {
                result += ", " + std::to_string(i + 1);
            }
        }
    }
    result += "]";
    return result;
}
