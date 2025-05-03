#include "Timer.hpp"
#include <cmath>
#include <cstdio>

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

    // Timer has not stopped
    this->stopped = true;
}

double Timer::get_duration() const {
    // If the timer has not started, return a minimum value of 1e-9 (1 nanosecond)
    // Done to prevent division by zero
    if (!this->started) {
        return 1e-9;
    }

    /** The most recent point in time from which to calculate the time elsapsed since the timer started */
    std::chrono::system_clock::time_point most_recent_time;

    if (this->stopped) {  // If the timer has stopped, use the time at which it was stopped
        most_recent_time = this->end_timestamp;
    } else {  // If the timer is still running, use the current time
        most_recent_time = std::chrono::high_resolution_clock::now();
    }

    // Get elapsed time
    const std::chrono::duration<double> elapsed_time = most_recent_time - this->start_timestamp;

    // Return the elapsed time in seconds ensuring a minimum value of 1e-9 (1 nanosecond)
    return std::max(1e-9, elapsed_time.count());
}

bool Timer::has_started() const {
    return this->started;
}

bool Timer::has_stopped() const {
    return this->stopped;
}
