#pragma once

#include "AbstractOdometry.hpp"
#include "../../Timer.hpp"

class OdometryArc : public AbstractOdometry {
public:
    /**
     * Constructor for the Odometry class.
     * @param initial_pose The initial pose of the robot.
     */
    explicit OdometryArc(const Pose& initial_pose);

    /**
     * Calculate the pose of the robot using motor encoders.
     * @param positions The left and right positions of the axles in degrees.
     * @return Returns the pose of the robot after calculating it.
     */
    Pose calculate(const std::pair<double, double> &positions);

    void set_pose(Pose new_pose) override;

    /** Deconstructor for the Odometry class. */
    ~OdometryArc() override;

protected:
    /** Most recent position of the left motor */
    double left_position = 0;
    /** Most recent position of the right motor */
    double right_position = 0;

    /** Weather or not the robot's pose was updated. At the time of writing, this is used to update
     * the heading of the robot after a reset */
    bool pose_updated = false;

    /** Left position before resetting the pose. This will be subtracted from all positions
     * to get the relative position of the robot. */
    double reset_position_left = 0;
    /** Right position before resetting the pose. This will be subtracted from all positions
     * to get the relative position of the robot. */
    double reset_position_right = 0;
    /** The heading the robot's pose was reset to. */
    double reset_heading = 0;

    /** total distance left side of robot has travelled (inches) */
    double left_distance = 0; // total distance left side of robot has travelled (inches)
    /** total distance right side of robot has travelled (inches) */
    double right_distance = 0;

    /** difference between last cycle and this cycle for left side distance (inches) */
    double delta_left = 0;
    /** difference between last cycle and this cycle for right side distance (inches) */
    double delta_right = 0;
    /** the average difference between the last cycle and this cycle for both sides (inches)
     * This represents the total distance traveled by the robot over the course of the last cycle. */
    double delta_avg = 0;

    /** the difference in heading between the last cycle and this cycle (degrees) */
    double delta_heading = 0;

    /** Timer used to record elapsed time between calls to <code>calculate()</code>. At the time of
     * writing, it used to calculate angular velocity. */
    Timer timer;

    /**
     * Updates the delta values.
     * Deltas are measures in inches.
     */
    virtual void update_deltas();

    /**
     * Update the total distances travelled.
     * Distance is measured in inches.
     */
    void update_distance();

    /**
     * Updated the heading of the robot using the left and right distances.
     */
    virtual void update_heading();

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
