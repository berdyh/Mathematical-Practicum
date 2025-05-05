#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Mapra libraries
#include "../include/mapra_test.h"
#include "../include/unit.h"

// Algorithms
#include "../include/bubblesort.h"
#include "../include/insertionsort.h"
#include "../include/selectionsort.h"

struct Sorter {
  std::string name;
  std::function<void(std::vector<unsigned int>&)> fn;
};

int main() {
  std::vector<Sorter> algorithms = {
      {"BubbleSort", mapra::Bubblesort},
      {"SelectionSort", mapra::Selectionsort},
      {"InsertionSort", mapra::Insertionsort},
      // Add others here
  };
  mapra::MapraTest test("AllSorters");

  for (const auto& sorter : algorithms) {
    for (unsigned int id = 0; id < mapra::kNumExamples; ++id) {
      auto arr = mapra::GetExample(id);
      sorter.fn(arr);
      test.Assert(sorter.name + " example " + std::to_string(id),
                  mapra::CheckSolution(arr));
    }
  }

  return 0;
}
