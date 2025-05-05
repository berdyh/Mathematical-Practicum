#include <string>

#include "../include/bubblesort.h"
#include "../include/mapra_test.h"
#include "../include/unit.h"

int main() {
  mapra::MapraTest test("BubbleSort");

  for (unsigned int id = 0; id < mapra::kNumExamples; ++id) {
    auto arr = mapra::GetExample(id);
    mapra::Bubblesort(arr);
    test.Assert("BubbleSort example " + std::to_string(id),
                mapra::CheckSolution(arr));
  }

  return 0;
}
