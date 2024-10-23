#include "Timer.hpp"
#include <cmath>

void Timer::start() {
    this->start_timestamp = std::chrono::high_resolution_clock::now();
    this->started = true;
}

void Timer::stop() {
    this->end_timestamp = std::chrono::high_resolution_clock::now();
}

double Timer::get_duration() {
    if (!this->started) {
        return 1e-9f;
    }
    return std::max(((double)(std::chrono::duration_cast<std::chrono::nanoseconds>(this->end_timestamp - this->start_timestamp).count())) / 1e9, 1e-9);
}

bool Timer::has_started() const {
    return this->started;
}

void Timer::reset() {
    if (!this->started) {
        this->start_timestamp = std::chrono::high_resolution_clock::now();
        this->started = true;
    }
    this->start_timestamp = this->end_timestamp;
}