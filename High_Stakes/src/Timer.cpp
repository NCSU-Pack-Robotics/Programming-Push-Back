#include "Timer.hpp"
#include <cmath>

void Timer::start() {
    // Save current time as start time
    this->start_timestamp = std::chrono::high_resolution_clock::now();

    // Timer has started and not stopped
    this->started = true;
    this->stopped = false;
}

void Timer::stop() {
    // Save current time as end time
    this->end_timestamp = std::chrono::high_resolution_clock::now();

    // Timer has stopped
    this->stopped = true;
}

double Timer::get_duration() {
    // If the timer has not started, return a minimum value of 1e-9 (1 nanosecond)
    // Done to prevent division by zero
    if (!this->started) {
        return 1e-9;
    }

    // If the timer has not stopped, stop the timer
    if (!this->stopped) {
        this->stop();
    }

    // Get elapsed time
    std::chrono::duration<double> elapsed_time = this->end_timestamp - this->start_timestamp;

    // Return the elapsed time in seconds ensuring a minimum value of 1e-9 (1 nanosecond)
    return std::max(1e-9, elapsed_time.count());
}

bool Timer::has_started() const {
    return this->started;
}

bool Timer::has_stopped() const {
    return this->stopped;
}
