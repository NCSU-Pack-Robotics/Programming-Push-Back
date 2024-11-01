#pragma once

/**
 * Namespace for PID constants used to set drive train velocity.
 * # TODO: Tune
 */
namespace Constants::PID::Drive::Velocity {
    /** PID Proportional coefficient for position calculations */
    constexpr double Kp = 0.0;
    /** PID Derivative coefficient for position calculations */
    constexpr double Kd = 0.0;
    /** PID Integral coefficient for position calculations */
    constexpr double Ki = 0.0;
}
