#pragma once

#include <chrono>

class Timer {
protected:
    std::chrono::system_clock::time_point start_timestamp;
    std::chrono::system_clock::time_point end_timestamp;
    bool started = false;

public:
    /**
     * Start timing
     */
    void start();

    /**
     * Stop timing
     */
    void stop();

    /**
     * Get the duration for which the timer ran (in seconds) floored to the nanosecond
     * @return Duration with a minimum value of 1e-9 (1 nanosecond), so it is always safe to use as a divisor
     */
    [[nodiscard]] double get_duration();

    /**
     * @return Whether or not the timer has started
     */
    [[nodiscard]] bool has_started() const;

    void reset();
};