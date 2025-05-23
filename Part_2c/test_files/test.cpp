#include <iostream>
#include <vector>

#include "../include/bubblesort.h"
#include "../include/io.h"
#include "../include/mergesort.h"
#include "../include/selectionsort.h"
#include "../include/student.h"

int main() {
  // --- 1) Doubles ---
  std::vector<double> vd{3.14, 2.71, -1.0, 42.0, 2.71};
  std::cout << "Before (double):\n";
  for (auto x : vd) std::cout << x << " ";
  std::cout << "\n";
  mapra::Bubblesort(vd);
  std::cout << "After Bubblesort:\n";
  for (auto x : vd) std::cout << x << " ";
  std::cout << "\n\n";

  // --- 2) Strings ---
  std::vector<std::string> vs{"pear", "apple", "Banana", "banana"};
  std::cout << "Before (string):\n";
  for (auto& s : vs) std::cout << s << " ";
  std::cout << "\n";
  mapra::Selectionsort(vs);
  std::cout << "After Selectionsort:\n";
  for (auto& s : vs) std::cout << s << " ";
  std::cout << "\n\n";

  // --- 3) Students ---
  std::vector<mapra::Student> vst{{"Carl-Friedrich", "Gauss", 111111, 1.0},
                                  {"Karl", "Weierstrass", 123456, 1.3},
                                  {"Pierre-Louis", "Lions", 301979, 1.3},
                                  {"Fritzchen", "Mueller", 222222, 4.0}};
  std::cout << "Before (Student):\n";
  for (auto& st : vst) std::cout << st << "\n";
  std::cout << "\n";
  mapra::Mergesort(vst);
  std::cout << "After Mergesort:\n";
  for (auto& st : vst) std::cout << st << "\n";

  return 0;
}
