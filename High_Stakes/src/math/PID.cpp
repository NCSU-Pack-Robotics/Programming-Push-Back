#include "PID.hpp"

double PID::calculate(const double error) {
    // If first loop, start the timer and return the proportional term
    if (this->first_loop) {
        this->timer.start();
        this->first_loop = false;

        // No delta time to calculate derivative or integral for in the first loop
        return this->calculate_proportional(error);
    }

    // Calculate the time elapsed since the last call
    const double delta_time = this->timer.get_duration();
    this->timer.start();  // Reset the timer for the next iteration

    // Calculate the PID outputs
    const double p = this->calculate_proportional(error);
    const double i = this->calculate_integral(error, delta_time);
    const double d = this->calculate_derivative(error, delta_time);

    // Return the sum of the three terms
    return p + i + d;
}

double PID::calculate_proportional(const double error) const {
    // Multiply by the proportional coefficient
    return this->k_proportional * error;
}

double PID::calculate_integral(const double error, const double delta_time) {
    // Integral term is the sum of all errors over time
    this->integral += error * delta_time;

    // Multiply by the integral coefficient
    return this->k_integral * this->integral;
}

double PID::calculate_derivative(const double error, const double delta_time) {
    // Derivative term is the rate of change of the error (slope)
    const double derivative = (error - this->last_error) / delta_time;
    this->last_error = error;  // Update the last error for the next iteration

    // Multiply by the derivative coefficient
    return this->k_derivative * derivative;
}
