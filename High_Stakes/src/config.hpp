#pragma once

#include <cstdint>

/**
 * This namespace is to be used for the starting configuration of the robot.
 * Specifically, this namespace is for the initial pose of the robot.
 */
namespace Constants::Initial::Pose {
    /** The initial x position of the robot */
    constexpr double INITIAL_X = 0;
    /** The initial y position of the robot */
    constexpr double INITIAL_Y = 0;
    /** The initial heading of the robot */
    constexpr double INITIAL_HEADING = 0;
}
