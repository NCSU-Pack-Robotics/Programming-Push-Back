#pragma once

#include <queue>

#include "../Timer.hpp"

/**
 * A PID controller is a control loop feedback mechanism used to minimize some error
 * over time by adjusting a control input based on the error value.
 * Popular in robotics, a PID controller is used to control the speed of motors.
 *
 * P: Proportional  (compensates for present error—absolutely essential)
 * I: Integral  (compensates for steady state error—very common to ignore)
 * D: Derivative  (compensates for overshoot—often optional but useful)
 */
class PID {
    /** Proportional coefficient */
    double k_proportional;
    /** Integral coefficient */
    double k_integral;
    /** Derivative coefficient */
    double k_derivative;

    /** Timer to calculate elapsed time between calls */
    Timer timer;

    /** Flag to indicate if this is the first loop */
    bool first_loop = true;

    /** The sum of errors over time */
    double integrator = 0;

    /** A queue of integrals summed to calculate the integrator */
    std::queue<double> integrators;

    /** The last value of the error */
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
    PID(const double k_proportional, const double k_integral, const double k_derivative)
        : k_proportional(k_proportional), k_integral(k_integral), k_derivative(k_derivative) {}

    /**
     * Calculate new output from given error
     * @param error Displacement from target state
     * @return PID output
     */
    double calculate(double error);

private:
    /**
     * Calculate the proportional term
     * @param error Displacement from target state
     * @return Proportional term
     */
    double calculate_proportional(double error) const;

    /**
     * Calculate the integral term
     * @param error Displacement from target state
     * @param delta_time Time elapsed since last call
     * @return Integral term
     */
    double calculate_integral(double error, double delta_time);

    /**
     * Calculate the derivative term
     * @param error Displacement from target state
     * @param delta_time Time elapsed since last call
     * @return Derivative term
     */
    double calculate_derivative(double error, double delta_time);
};
