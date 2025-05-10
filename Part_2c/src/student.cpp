// Copyright (c) 2022, The MaPra Authors.

#include "../include/student.h"

// Eingabeoperator ">>"
std::istream& mapra::operator>>(std::istream& s, mapra::Student& a) {
  s >> a.first_name >> a.last_name >> a.matr_nr >> a.grade;
  return s;
}

// Ausgabeoperator "<<"
std::ostream& mapra::operator<<(std::ostream& s, const mapra::Student& a) {
  s << a.first_name << " " << a.last_name << " " << a.matr_nr << " " << a.grade;
  return s;
}

// Vergleichsoperator "<"
bool mapra::operator<(const mapra::Student& lhs, const mapra::Student& rhs) {
  if (lhs.last_name < rhs.last_name) return true;
  if (rhs.last_name < lhs.last_name) return false;
  return lhs.first_name < rhs.first_name;
}

// Vergleichsoperatoren "==" bzw. "!="
bool mapra::operator==(const mapra::Student& lhs, const mapra::Student& rhs) {
  return lhs.first_name == rhs.first_name && lhs.last_name == rhs.last_name &&
         lhs.matr_nr == rhs.matr_nr && lhs.grade == rhs.grade;
}

bool mapra::operator!=(const mapra::Student& lhs, const mapra::Student& rhs) {
  return !(lhs == rhs);
}

// Vergleichsoperator ">"
bool mapra::operator>(const mapra::Student& lhs, const mapra::Student& rhs) {
  return rhs < lhs;
}

// Vergleichsoperator "<="
bool mapra::operator<=(const mapra::Student& lhs, const mapra::Student& rhs) {
  return (lhs < rhs || lhs == rhs);
}

// Vergleichsoperator ">="
bool mapra::operator>=(const mapra::Student& lhs, const mapra::Student& rhs) {
  return (lhs > rhs || lhs == rhs);
}
