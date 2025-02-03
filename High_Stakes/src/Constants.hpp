#pragma once

#include <cstdint>

/**
 * Namespace for constants used throughout the codebase.
 * These constants are stagnant for each robot.
*/
namespace Constants {

    /**
     * Enum for the type of drive control to use.
     * VOLTAGE: Set the voltage of the motors directly.
     * POWER: Set the power of the motors from analog sticks.
    */
    enum DriveType {
        /** Set the voltage of the motors directly. */
        VOLTAGE,
        /** Set the power of the motors from analog sticks. */
        POWER
    };

    namespace Math {
        /** 90 degrees */
        constexpr double HALF_PI = 1.57079632679489661923;

        /** 180 degrees */
        constexpr double PI = 3.14159265358979323846;

        /** 360 degrees */
        constexpr double TAU = 6.28318530717958647692;
    }

    namespace Hardware {
        /**
         * The diameter of a tracking wheel
         *     • Measured between outer omni rollers
         *     • Measured in inches
        */
        constexpr double TRACKING_DIAMETER = 3.1875;

        /** Ratio of tracking wheel upper gear to lower wheel gear */
        constexpr double TRACKING_RATIO = 36.0 / 60.0;

        /**
         * The diameter of a tracking wheel
         *     • Measured between outer omni rollers
         *     • Measured in inches
        */
        constexpr double ROBOT_DIAMETER = 12.46875;

        /**
         * Half the distance between the centers of each tracking wheel
         *      • Measured in inches
        */
        constexpr double ROBOT_RADIUS = ROBOT_DIAMETER / 2;

        /** Maximum motor voltage in milli-volts */
        constexpr int32_t MAX_MOTOR_VOLTAGE = 12'000;

        /**
         * Maximum motor power.
         *     • Value is unless and corresponds to the min/max analog stick values
         *     • Used when setting motor power
        */
        constexpr int32_t MAX_MOTOR_POWER = 127;
    }

    /**
     * Constants used for any sort of path following algorithm.
     * At the time of writing, this only pertains to pure pursuit, but could include others as well.
     */
    namespace PathFollowing {
        /** In pure pursuit, the distance from the robot to the lookahead point */
        constexpr double LOOKAHEAD_DISTANCE = 12;

        /** In pure pursuit, the distance from the target point before the robot stops */
        constexpr double STOP_DISTANCE = Hardware::ROBOT_RADIUS;
    }

}
