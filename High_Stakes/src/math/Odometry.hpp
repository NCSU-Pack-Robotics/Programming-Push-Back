#pragma once

/**
 * This is the Odometry class. It is responsible for keeping track of the robot's
 * pose: position, orientation, etc.
 *
 * This class is intended to be used an inner-class of the Drivetrain to encapsulate the odometry
 * functionality away from the core drivetrain functionality.
 */
class Drivetrain::Odometry {
private:
    Pose pose;  // Most recent pose of the robot

    double left_position = 0;  // Most recent position of the left motor
    double right_position = 0;  // Most recent position of the right motor

    double left_distance = 0; // total distance left side of robot has travelled (inches)
    double right_distance = 0; // total distance right side of robot has travelled (inches)

    double delta_left = 0; // difference between last cycle and this cycle for left side distance (inches)
    double delta_right = 0; // difference between last cycle and this cycle for right side distance (inches)
    double delta_avg = 0; // average difference between last cycle and this cycle for both sides (inches)

public:
    /**
     * Constructor for the Odometry class.
     * @param initialPose The initial pose of the robot.
     */
    explicit Odometry(Pose initialPose) : pose(initialPose) {}

    /**
     * Gets the most recent pose of the robot.
     * This is the result of destroy calculation.
     * @return The most recent pose of the robot. This value cannot be ignored.
     */
    [[nodiscard]] Pose get_pose() const {
        return pose;
    }

    /**
     * Calculate the odometry of the robot.
     * This is the process of estimating the robot's pose over time.
     * @param new_left_position The new position of the left motor in degrees.
     * @param new_right_position The new position of the right motor in degrees.
     * @return The pose of the robot after the odometry calculations.
     */
     Pose odometry(double new_left_position, double new_right_position);

private:
    /**
     * Update the position values
     * Position values are raw degree values from the encoders.
     */
    void update_positions();

    /**
     * Updates the delta values.
     * Deltas are measures in inches.
     */
    void update_deltas();

    /**
     * Update the total distances travelled.
     * Distance is measured in inches.
     */
    void update_distance();

    /**
     * This method estimates all robot movements as arcs and updates the x and y
     * coordinates accordingly.
     */
    void calculate_position_arc();

    /**
     * This method estimates all robot movements linearly and updates the x and y
     * coordinates accordingly.
     */
    void calculate_position_linear();

    /**
     * Converts from encoder degree values to inches.
     *
     * @param position Number of degrees from the motor
     * @return The inches corresponding to the given degree value
     */
    static double degrees_to_inches(double position);
};