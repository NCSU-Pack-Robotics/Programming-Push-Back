#pragma once

#include "AbstractOdometry.hpp"

class OdometryArc final : public AbstractOdometry {
public:
    /**
     * Constructor for the Odometry class.
     * @param initial_pose The initial pose of the robot.
     */
    explicit OdometryArc(Pose initial_pose);

    /**
     * Calculate the pose of the robot using motor encoders.
     * @param positions The left and right positions of the axles in degrees.
     * @return Returns the pose of the robot after calculating it.
     */
    Pose calculate(const std::pair<double, double> &positions);

    /** Deconstructor for the Odometry class. */
    ~OdometryArc() override;

protected:
    /** Most recent position of the left motor */
    double left_position = 0;
    /** Most recent position of the right motor */
    double right_position = 0;

    /** total distance left side of robot has travelled (inches) */
    double left_distance = 0; // total distance left side of robot has travelled (inches)
    /** total distance right side of robot has travelled (inches) */
    double right_distance = 0;

    /** difference between last cycle and this cycle for left side distance (inches) */
    double delta_left = 0;
    /** difference between last cycle and this cycle for right side distance (inches) */
    double delta_right = 0;
    /** average difference between last cycle and this cycle for both sides (inches) */
    double delta_avg = 0;

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
};
