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
    // Integral is the sum of the error over time
    const double integral = error * delta_time;

    this ->integrators.push(integral);  // Add the new integral to the queue
    this->integrator += integral;  // Add the new integral to the sum

    // Limit the size of the queue
    if (this->integrators.size() > 200) {  // At a 5ms delay, size 200 is 1 second worth of errors
        // Remove the oldest integral
        this-> integrator -= this->integrators.front();
        this->integrators.pop();
    }

    // Multiply by the integral coefficient
    return this->k_integral * this->integrator;
}

double PID::calculate_derivative(const double error, const double delta_time) {
    // Derivative term is the rate of change of the error (slope)
    const double derivative = (error - this->last_error) / delta_time;
    this->last_error = error;  // Update the last error for the next iteration

    // Multiply by the derivative coefficient
    return this->k_derivative * derivative;
}
