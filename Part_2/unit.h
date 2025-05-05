// Copyright (c) 2022, The MaPra Authors.

#ifndef UNIT_H_
#define UNIT_H_

#include <iostream>
#include <utility>
#include <vector>

namespace mapra {

// Konstanten
inline constexpr unsigned int kNumExamples = 4;

// Funktionen
std::vector<unsigned int> GetExample(unsigned int example_id,
                                     std::size_t length = 0);
bool CheckSolution(const std::vector<unsigned int>& array);
// use std::swap(array[i], array[j]) to swap element i and j

}  // namespace mapra

#endif  // UNIT_H_
