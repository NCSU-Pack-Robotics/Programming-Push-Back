#pragma once

#include "Timer.hpp"

/**
 * P: Proportional
 * I: Integral
 * D: Derivative
 */
class PID {
protected:
    const double k_proportional;
    const double k_integral;
    const double k_derivative;

    Timer timer;
    bool first_loop = true;

    double integral = 0;
    double last_error = 0;

public:
    /**
     * P: Proportional
     * I: Integral
     * D: Derivative
     * @param k_proportional Proportional coefficient
     * @param k_integral Integral coefficient
     * @param k_derivative Derivative coefficient
     */
    constexpr PID(const double k_proportional, const double k_integral, const double k_derivative): k_proportional(k_proportional), k_integral(k_integral), k_derivative(k_derivative) {}

    /**
     * Calculate new output from given error
     * @param error Displacement from target state
     * @return PID output
     */
    double calculate(double error);
};