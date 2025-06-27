// Copyright (c) 2022, The MaPra Authors.

#include <iostream>
#include <cmath>
#include <vector>
#include "mapra/unit.h"

namespace {

// Step size control helper
real ComputeStepSizeMultiplier(real error_norm) {
    const real safety_factor = 0.9;
    const real min_factor = 0.1;
    const real max_factor = 5.0;
    
    real factor;
    if (error_norm > 0) {
        factor = safety_factor * std::pow(mapra::eps / error_norm, 1.0 / (mapra::rk_p + 1.0));
    } else {
        factor = max_factor;
    }
    
    return std::max(min_factor, std::min(max_factor, factor));
}

// Multi-body RHS computation
RealVector ComputeMultiBodyRHS(real t, const RealVector& y, const RealVector& masses) {
    const std::size_t n_bodies = masses.GetLength();
    const std::size_t dim = 2;
    RealVector dydt(y.GetLength());
    
    // Copy velocities to position derivatives
    for (std::size_t i = 0; i < n_bodies * dim; ++i) {
        dydt(i) = y(n_bodies * dim + i);
    }
    
    // Compute gravitational accelerations
    for (std::size_t i = 0; i < n_bodies; ++i) {
        real ax = 0, ay = 0;
        
        for (std::size_t j = 0; j < n_bodies; ++j) {
            if (i != j) {
                real dx = y(j * dim) - y(i * dim);
                real dy = y(j * dim + 1) - y(i * dim + 1);
                real r_squared = dx * dx + dy * dy;
                real r = std::sqrt(r_squared);
                real r_cubed = r_squared * r;
                
                real force_factor = mapra::kGrav * masses(j) / r_cubed;
                ax += force_factor * dx;
                ay += force_factor * dy;
            }
        }
        
        dydt(n_bodies * dim + i * dim) = ax;
        dydt(n_bodies * dim + i * dim + 1) = ay;
    }
    
    return dydt;
}

// Main Runge-Kutta step function
void RkStep(VFFunction f, real& t, RealVector& y, real& h) {
    const std::size_t stages = mapra::rk_alpha.GetLength();
    std::vector<RealVector> k(stages);
    
    bool step_accepted = false;
    real h_current = h;
    
    while (!step_accepted) {
        // Compute Runge-Kutta stages
        for (std::size_t i = 0; i < stages; ++i) {
            RealVector y_temp = y;
            for (std::size_t j = 0; j < i; ++j) {
                y_temp += mapra::rk_beta(i, j) * h_current * k[j];
            }
            k[i] = f(t + mapra::rk_alpha(i) * h_current, y_temp);
        }
        
        // Compute both method solutions
        RealVector y_new = y;
        RealVector y_embedded = y;
        
        for (std::size_t i = 0; i < stages; ++i) {
            y_new += mapra::rk_gamma(i) * h_current * k[i];
            y_embedded += mapra::rk_delta(i) * h_current * k[i];
        }
        
        // Error control
        RealVector error = y_new - y_embedded;
        real error_norm = error.NormMax();
        
        if (error_norm <= mapra::eps) {
            step_accepted = true;
            y = y_new;
            t += h_current;
        }
        
        h_current *= ComputeStepSizeMultiplier(error_norm);
        if (step_accepted) h = h_current;
    }
}

} // anonymous namespace

int main() {
    using namespace mapra;
    
    for (int ex_id = 1; ex_id <= kNumExamples; ++ex_id) {
        std::cout << "Solving example " << ex_id << std::endl;
        
        auto data = GetExample(ex_id, false, true);
        
        // Determine RHS function
        VFFunction rhs_function;
        bool has_masses = false;
        for (std::size_t i = 0; i < data.mass.GetLength(); ++i) {
            if (data.mass(i) != 0.0) {
                has_masses = true;
                break;
            }
        }
        
        if (has_masses) {
            rhs_function = [&data](real t, const RealVector& y) -> RealVector {
                return ComputeMultiBodyRHS(t, y, data.mass);
            };
        } else {
            rhs_function = data.fun;
        }
        
        // Integration
        real t = data.t_begin;
        RealVector y = data.y_0;
        real h = data.h_0;
        
        CheckStep(t, y, true, true);
        
        while (t < data.t_end) {
            if (t + h > data.t_end) h = data.t_end - t;
            RkStep(rhs_function, t, y, h);
            CheckStep(t, y, true, true);
        }
        
        CheckSolution(t, y);
        std::cout << "Example " << ex_id << " completed\n";
    }
    
    return 0;
}
