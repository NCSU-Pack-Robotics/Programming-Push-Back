#pragma once

/**
 * Represents the pose of the robot.
 * In robotics, pose to the current state of a robot in the world.
 * The pose usually consists of the position and orientation of the robot.
 */
struct Pose {
    /** The x-coordinate of the robot in inches. */
    double x;
    /** The y-coordinate of the robot in inches. */
    double y;
    /** The heading of the robot in radians. */
    double heading;
};
