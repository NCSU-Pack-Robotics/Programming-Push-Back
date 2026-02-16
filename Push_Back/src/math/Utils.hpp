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
        return rpm * Constants::Hardware::TRACKING_DIAMETER * M_PI *
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
               M_PI * (position / 360);
    }

    /**
     * Ensures that the given angle is between 0 and 2 * PI radians.
     * @return The normalized angle in radians.
     */
    static inline double ensure_positive_radians(double radians) {
        radians = fmod(radians, M_TWOPI);
        if (radians < 0) {
            radians += M_TWOPI;
        }

        return radians;
    }
};
