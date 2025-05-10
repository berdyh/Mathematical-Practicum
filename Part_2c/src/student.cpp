// Copyright (c) 2022, The MaPra Authors.

#include "student.h"

// Eingabeoperator ">>"
std::istream& mapra::operator>>(std::istream& s, mapra::Student& a) {
  s >> a.first_name >> a.last_name >> a.matr_nr >> a.grade;
  return s;
}

// Ausgabeoperator "<<"
std::ostream& mapra::operator<<(std::ostream& s, const mapra::Student& a) {
  // hier den Ausgabeoperator definieren
}

// Vergleichsoperator "<"
bool mapra::operator<(const mapra::Student&, const mapra::Student&) {
  // hier fehlt was
}

// Vergleichsoperatoren "==" bzw. "!="
bool mapra::operator==(const mapra::Student&, const mapra::Student&) {
  // hier fehlt was
}

bool mapra::operator!=(const mapra::Student&, const mapra::Student&) {
  // hier fehlt was
}
