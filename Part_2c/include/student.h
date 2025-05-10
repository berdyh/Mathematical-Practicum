// Copyright (c) 2022, The MaPra Authors.

#ifndef STUDENT_H_
#define STUDENT_H_

#include <iostream>
#include <string>

namespace mapra {

struct Student {
  std::string first_name;
  std::string last_name;
  int matr_nr;
  double grade;
};
// Eingabeoperator ">>"
std::istream& operator>>(std::istream&, Student&);

// Ausgabeoperator "<<"
std::ostream& operator<<(std::ostream&, const Student&);

// Vergleichsoperator "<"
bool operator<(const Student&, const Student&);

// Vergleichsoperatoren "==" bzw. "!="
bool operator==(const Student&, const Student&);
bool operator!=(const Student&, const Student&);

}  // namespace mapra

#endif  // STUDENT_H_
