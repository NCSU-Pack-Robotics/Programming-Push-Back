#pragma once

#include "Pose.hpp"

/**
 * This is the AbstractOdometry class. It is responsible for keeping track of the robot's
 * pose: position, orientation, etc. Call concrete implementation of this class with some flavor of
 * <code>Odometry.calculate()</code> method to update the pose.
 * <p>
 * This is an abstract class from which concrete odometry implementations can be implemented.
 * <p>
 * Relative to the robot, the positive x-axis is forward and the positive y-axis is to the left.
 * Turning left (counter-clockwise) is positive, and turning right (clockwise) is negative.
 */
class AbstractOdometry {
public:
    /** Most recent pose of the robot */
    Pose pose;

    /**
     * Constructor for the Odometry class.
     * @param initial_pose The initial pose of the robot.
     */
    explicit AbstractOdometry(const Pose& initial_pose);

    /**
     * Gets the most recent pose of the robot.
     * This is the result of odometry calculation.
     * @return The most recent pose of the robot. If the method is called, it's return value cannot
     * be ignored.
     */
    [[nodiscard]] Pose get_pose() const;

    /**
     * Sets the pose of the robot.
     * This method is intended to be used when the robot hits a wall to realign itself.
     * @param new_pose The new pose of the robot.
     */
    virtual void set_pose(Pose new_pose);

    /**
     * Deleted default constructor for the AbstractOdometry class.
     * This constructor is deleted to prevent instantiation without parameters.
     */
    AbstractOdometry() = delete;

    /**
     * Destructor for the AbstractOdometry class.
     * This destructor is virtual to allow derived classes to clean up properly.
     */
    virtual ~AbstractOdometry() = default;
};