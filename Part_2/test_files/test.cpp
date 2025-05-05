#include "../include/bubblesort.h"
#include "../include/heapsort.h"
#include "../include/insertionsort.h"
#include "../include/quicksort_simple.h"
#include "../include/selectionsort.h"
// #include others

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Sorter {
  std::string name;
  std::function<void(std::vector<unsigned int>&)> fn;
};

// Manual test cases: input → expected sorted output
const std::vector<
    std::pair<std::vector<unsigned int>, std::vector<unsigned int>>>
    manual_tests = {
        {{}, {}},
        {{7}, {7}},
        {{2, 1}, {1, 2}},
        {{1, 1, 1}, {1, 1, 1}},
        {{1, 2, 3, 2, 5}, {1, 2, 2, 3, 5}},
        {{5, 4, 3, 2, 1}, {1, 2, 3, 4, 5}},
};

void RunManualTests(const Sorter& sorter) {
  std::cout << "--- " << sorter.name << " ---\n";

  for (std::size_t i = 0; i < manual_tests.size(); ++i) {
    auto input = manual_tests[i].first;
    const auto& expected = manual_tests[i].second;

    sorter.fn(input);
    bool ok = (input == expected);

    std::cout << "  test " << i << ": " << (ok ? "PASS" : "FAIL") << "\n";
    if (!ok) {
      std::cout << "    expected: ";
      for (auto x : expected) std::cout << x << " ";
      std::cout << "\n    got:      ";
      for (auto x : input) std::cout << x << " ";
      std::cout << "\n";
    }

    assert(ok && "Manual test failed.");
  }

  std::cout << sorter.name << " passed all manual tests!\n\n";
}

int main() {
  std::vector<Sorter> algorithms = {
      {"BubbleSort", mapra::Bubblesort},
      {"SelectionSort", mapra::Selectionsort},
      {"InsertionSort", mapra::Insertionsort},
      {"HeapSort", mapra::Heapsort},
      {"QuicksortSimple", mapra::QuicksortSimple},
      // Add others here
  };

  for (const auto& s : algorithms) {
    RunManualTests(s);
  }

  return 0;
}
