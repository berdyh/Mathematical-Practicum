// Copyright (c) 2022, The MaPra Authors.

#ifndef UNIT_H_
#define UNIT_H_

#include <iostream>
#include <string>
#include <vector>

#include "student.h"

namespace mapra {

// Funktionen zur Ueberpruefung der Ergebnisse
// geben zurueck, ob ein Fehler entdeckt wurde
bool CheckSolution(const std::vector<double>& array);
bool CheckSolution(const std::vector<std::string>& array);
bool CheckSolution(const std::vector<mapra::Student>& array);

}  // namespace mapra

#endif  // UNIT_H_
