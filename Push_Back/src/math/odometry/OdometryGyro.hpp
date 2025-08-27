#pragma once

#include "OdometryArc.hpp"

/**
 * Odometry Gyra uses most of the implementation from OdometryArc, however, it expects heading
 * values to be given when calculating new positions. At the time of writing, this is so that
 * a gyro sensor can calculate the heading and inject it into the odometry
 */
class OdometryGyro final : public OdometryArc {
    /** The heading given to the <code>calculate</code> method. */
    double injected_heading = 0;
    /** The last heading given to the <code>calculate</code> method. */
    double last_injected_heading = 0;

    double initial_pose = 0;

public:
    /**
     * Constructor for the OdometryGyro class.
     * @param initial_pose The initial pose of the robot.
     */
    explicit OdometryGyro(const Pose& initial_pose);

    /**
     * Calculate the pose of the robot using motor encoders and a heading.
     * @param positions The left and right positions of the axles in degrees.
     * @param heading The heading of the robot in degrees.
     * @return Returns the pose of the robot after calculating it.
     */
    Pose calculate(const std::pair<double, double> &positions, double heading);

private:
    /** Updates the heading of the robot using the injected heading. */
    void update_heading() override;

    void update_deltas() override;
};
