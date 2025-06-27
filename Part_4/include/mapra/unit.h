// Copyright (c) 2022, The MaPra Authors.

#ifndef UNIT_H_
#define UNIT_H_

#include <functional>

#include "mapra/matrix.h"
#include "mapra/vector.h"

using real = long double;
using RealVector = mapra::Vector<real>;
using RealMatrix = mapra::Matrix<real>;

// Funktionstyp zum Uebergeben der rechten Seite der DGL
using VFFunction = std::function<mapra::Vector<real>(
    real, const mapra::Vector<real>&)>;

namespace mapra {

// Anzahl der Beispielfunktionen
inline constexpr int kNumExamples = 8;

// Gravitationskonstante
inline constexpr real kGrav = 6.67e-11;

// Koeffizienten fuer das Runge-Kutta-Verfahren
extern RealVector rk_alpha, rk_gamma, rk_delta;
extern RealMatrix rk_beta;
extern unsigned int rk_p;  // Ordnung
extern real eps;

struct ExampleData {
  RealVector mass;
  VFFunction fun;
  RealVector y_0;
  real t_begin;
  real t_end;
  real h_0;
};

ExampleData GetExample(int ex_id, bool doDraw, bool doOutput = true);

void CheckStep(real t, const RealVector& y, bool doDraw, bool doOutput = true);

void CheckSolution(real t, const RealVector& y);

}  // namespace mapra

#endif  // UNIT_H_
