#include "PID.hpp"


double PID::calculate(const double error) {
    if (this->first_loop) {
        this->first_loop = false;
        this->timer.start();
        return this->k_proportional * error;
    }

    const double delta_time = this->timer.get_duration();
    this->timer.reset();

    this->integral += error * delta_time;
    const float derivative = (error - this->last_error) / delta_time;
    this->last_error = error;
    return this->k_proportional * error + this->k_integral * this->integral + this->k_derivative * derivative;
}
