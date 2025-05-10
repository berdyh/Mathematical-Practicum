#include <fstream>
#include <iostream>
#include <vector>

// Mapra libraries
#include "../include/mapra_test.h"
#include "../include/unit.h"

// Include sort algorithms
#include "../include/bubblesort.h"
#include "../include/io.h"
#include "../include/mergesort.h"
#include "../include/selectionsort.h"
#include "../include/student.h"
#include "../include/unit.h"

int main() {
  // 1) Open files
  std::ifstream fd("doubles.txt");
  std::ifstream fs("strings.txt");
  std::ifstream fst("studenten.txt");
  if (!fd || !fs || !fst) {
    std::cerr << "Error: could not open input files\n";
    return 1;
  }

  // 2) Read into vectors
  std::vector<double> vd;  // for our sort routines
  std::vector<std::string> vs;
  std::vector<mapra::Student> vst;
  mapra::Read(fd, vd);
  mapra::Read(fs, vs);
  mapra::Read(fst, vst);

  // 3) Ask user which algorithm
  std::cout << "Which sort? B=Bubblesort, A=Selectionsort, M=Mergesort: ";
  char c;
  std::cin >> c;

  // 4) Dispatch
  switch (c) {
    case 'B':
    case 'b':
      mapra::Bubblesort(vd);
      mapra::Bubblesort(vs);
      mapra::Bubblesort(vst);
      break;
    case 'A':
    case 'a':
      mapra::Selectionsort(vd);
      mapra::Selectionsort(vs);
      mapra::Selectionsort(vst);
      break;
    case 'M':
    case 'm':
      mapra::Mergesort(vd);
      mapra::Mergesort(vs);
      mapra::Mergesort(vst);
      break;
    default:
      std::cerr << "Unknown choice\n";
      return 1;
  }

  // 5) Print results
  std::cout << "\n-- Sorted doubles --\n";
  mapra::Print(std::cout, vd);
  std::cout << "-- Sorted strings --\n";
  mapra::Print(std::cout, vs);
  std::cout << "-- Sorted students --\n";
  mapra::Print(std::cout, vst);

  // 6) Verify
  if (!mapra::CheckSolution(vd)) std::cerr << "Double sort failed!\n";
  if (!mapra::CheckSolution(vs)) std::cerr << "String sort failed!\n";
  if (!mapra::CheckSolution(vst)) std::cerr << "Student sort failed!\n";

  return 0;
}
