#pragma once
#include "../Constants.hpp"

/**
 * This class contains utility methods for the project.
 */
class Utils {
public:
    /**
     * Helper method to convert <code>Drivetrain</code> RPM to inches per second.
     *
     * @param rpm The RPM to convert to inches per second.
     * @return The RPM converted to inches per second.
     */
     static inline double rpm_to_ips(const double rpm) {
         return rpm * Constants::Hardware::TRACKING_DIAMETER * Constants::Math::PI *
           Constants::Hardware::TRACKING_RATIO / 60;
     }

    /**
     * Converts from <code>Drivetrain</code> encoder degree values to inches.
     *
     * @param position Number of degrees from the motor
     * @return The inches corresponding to the given degree value
     */
    static inline double degrees_to_inches(const double position) {
         return Constants::Hardware::TRACKING_RATIO * Constants::Hardware::TRACKING_DIAMETER *
               Constants::Math::PI * (position / 360);
     }
};

/**
 * An instance of the Utils class.
 * This contains utility methods for the project.
 */
static constexpr Utils utils;
