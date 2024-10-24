#pragma once

#include <chrono>

class Timer {
private:
    /** Time at which the timer was started. */
    std::chrono::system_clock::time_point start_timestamp;
    /** Time at which the timer was stopped. */
    std::chrono::system_clock::time_point end_timestamp;

    /** Whether or not the timer has started. */
    bool started = false;

    /** Whether or not the timer has stopped. */
    bool stopped = false;

public:

    /**
     * Start timing.
     * This method also acts as a reset for the timer.
     */
    void start();

    /**
     * Stop timing.
     */
    void stop();

    /**
     * Get the duration for which the timer has been running in seconds.
     * If the timer has not stopped, it will stop the timer.
     * If the timer has not started, it will return a minimum value of 1e-9 (1 nanosecond).
     *
     * @return Duration with a minimum value of 1e-9 (1 nanosecond), so it is always safe to use
     * as a divisor.
     */
    [[nodiscard]] double get_duration();

    /**
     * @return Whether or not the timer has started
     */
    [[nodiscard]] bool has_started() const;

    /**
     * @return Whether or not the timer has stopped
     */
    [[nodiscard]] bool has_stopped() const;
};
