#pragma once

#include <cstdint>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Pose.hpp"
#include "../math/PID.hpp"
#include "../Config.hpp"
#include "../Constants.hpp"

/**
 * The Drivetrain subsystem is responsible for controlling and reading from the drive motors.
 */
class Drivetrain : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /**
     * Set the voltage (in milli-volts) of the left and right motors in range [-12000,12000].
     * @param left_mV The voltage to set the left motors to.
     * @param right_mV The voltage to set the right motors to.
     */
    void set_voltage(int32_t left_mV, int32_t right_mV);

    /**
     * Set the power of the left and right motors in range [-127, 127].
     * @param left_power The power to set the left motors to.
     * @param right_power The power to set the right motors to.
     */
    void set_drive_power(int32_t left_power, int32_t right_power);

    /**
     * Set the left motors to run at a target velocity in inches per second
     * @param target_left_velocity The velocity of the left motors in inches per second
     * @param target_right_velocity The velocity of the right motors in inches per second
     */
    void set_velocity(double target_left_velocity, double target_right_velocity);

    /**
     * Stop the robot immediately.
     */
    void stop();

    /**
     * Get the position of the left and right motors in degrees.
     * @return A pair of the left and right motor positions in degrees. The first value is the left
     * motor position, and the second value is the right motor position. 
     */
    std::pair<double, double> get_position() const;

    /**
    * Get the pose of the robot.
    * This is the result of odometer calculations.d
    * @return The pose of the robot.
    */
    Pose get_pose() const;

private:
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t left_drive_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t right_drive_voltage = 0;

    /**
    * This is the Odometry class. It is responsible for keeping track of the robot's
    * pose: position, orientation, etc.
    *
    * Relative to the robot, the positive x-axis is forward and the positive y-axis is to the left.
    * Turning left (counter-clockwise) is positive, and turning right (clockwise) is negative.
    *
    * This class is intended to be used an inner-class of the Drivetrain to encapsulate the calculate
    * functionality away from the core drivetrain functionality.
    */
    class Odometry {
        Drivetrain &drivetrain; // A reference to the enclosing Drivetrain instance.
        Pose pose = {0, 0 ,0}; // Most recent pose of the robot

        double left_position = 0; // Most recent position of the left motor
        double right_position = 0; // Most recent position of the right motor

        double left_distance = 0; // total distance left side of robot has travelled (inches)
        double right_distance = 0; // total distance right side of robot has travelled (inches)

        // difference between last cycle and this cycle for left side distance (inches)
        double delta_left = 0;
        // difference between last cycle and this cycle for right side distance (inches)
        double delta_right = 0;
        // average difference between last cycle and this cycle for both sides (inches)
        double delta_avg = 0;

    public:
        /**
         * Constructor for the Odometry class.
         * @param initialPose The initial pose of the robot.
         * @param drivetrain The drivetrain subsystem.
         */
        explicit Odometry(Pose initialPose, Drivetrain &drivetrain)
            : drivetrain(drivetrain), pose(initialPose) {
        }

        /**
         * Gets the most recent pose of the robot.
         * This is the result of destroy calculation.
         * @return The most recent pose of the robot. This value cannot be ignored.
         */
        [[nodiscard]] Pose get_pose() const {
            return this->pose;
        }

        /**
         * Call this method to run Odometry. This method will update the robot's pose.
         * <p>
         * This is the process of estimating the robot's pose over time.
         * @return The pose of the robot after the calculate calculations.
         */
        Pose calculate() {
            // Preliminary calculations
            update_positions();
            update_deltas(); // Needs to be done after positions calculations
            update_distance(); // Needs to be done after delta calculations

            // Update coordinates with preferred method
            calculate_position_arc();

            // must be done after coordinate calculations
            this->pose.heading = (right_distance - left_distance) /
                Constants::Hardware::ROBOT_DIAMETER;

            return this->get_pose();
        }

    private:
        /**
         * Update the position values.
         * Position values are raw degree values from the encoders.
         */
        void update_positions() {
            this->left_position = drivetrain.get_position().first;
            this->right_position = drivetrain.get_position().second;
        }

        /**
         * Updates the delta values.
         * Deltas are measures in inches.
         */
        void update_deltas() {
            this->delta_left = degrees_to_inches(this->left_position) - left_distance;
            this->delta_right = degrees_to_inches(this->right_position) - right_distance;
            this->delta_avg = (delta_left + delta_right) / 2;
        }

        /**
         * Update the total distances travelled.
         * Distance is measured in inches.
         */
        void update_distance() {
            this->left_distance = degrees_to_inches(left_position);
            this->right_distance = degrees_to_inches(right_position);
        }

        /**
         * This method estimates all robot movements as arcs and updates the x and y
         * coordinates accordingly.
         */
        void calculate_position_arc() {
            // position calculations
            const double turning_radius = fabs(
                Constants::Hardware::ROBOT_RADIUS * (delta_right + delta_left)
                / (delta_right - delta_left));
            const double delta_heading = (delta_right - delta_left) /
                Constants::Hardware::ROBOT_DIAMETER;

            // variables to store changes in x andy y
            double delta_x = 0.0;
            double delta_y = 0.0;

            // Calculate the change in x and y
            if ((delta_right - delta_left) != 0) {
                // turning
                if (fabs(delta_right) - fabs(delta_left) > 0) {
                    // turning left
                    delta_x = turning_radius *
                              (cos(this->pose.heading - Constants::Math::HALF_PI + delta_heading)
                               - cos(this->pose.heading - Constants::Math::HALF_PI));
                    delta_y = turning_radius *
                              (sin(this->pose.heading - Constants::Math::HALF_PI + delta_heading)
                               - sin(this->pose.heading - Constants::Math::HALF_PI));
                } else if (fabs(delta_right) - fabs(delta_left) < 0) {
                    // turning right
                    delta_x = turning_radius *
                              (cos(this->pose.heading + Constants::Math::HALF_PI + delta_heading)
                               - cos(this->pose.heading + Constants::Math::HALF_PI));
                    delta_y = turning_radius *
                              (sin(this->pose.heading + Constants::Math::HALF_PI + delta_heading)
                               - sin(this->pose.heading + Constants::Math::HALF_PI));
                }
            } else {
                // not turning
                const double delta_average = (delta_left + delta_right) / 2;
                delta_x = delta_average * cos(this->pose.heading);
                delta_y = delta_average * sin(this->pose.heading);
            }

            // Update x and y coords
            this->pose.x += delta_x;

            this->pose.y += delta_y;
        }

        /**
         * This method estimates all robot movements linearly and updates the x and y
         * coordinates accordingly.
         */
        void calculate_position_linear() {
            this->pose.x += delta_avg * cos(this->pose.heading);
            this->pose.y += delta_avg * sin(this->pose.heading);
        }

        /**
         * Converts from encoder degree values to inches.
         *
         * @param position Number of degrees from the motor
         * @return The inches corresponding to the given degree value
         */
        static inline double degrees_to_inches(const double position) {
            return Constants::Hardware::TRACKING_RATIO * Constants::Hardware::TRACKING_DIAMETER *
                   Constants::Math::PI * (position / 360);
        }
    };

    // Pointer to calculate instance
    std::unique_ptr<Odometry> odometry;

    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    int32_t left_drive_power = 0;
    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    int32_t right_drive_power = 0;

    /** The PID used for left motors velocity */
    PID left_velocity_pid = PID(Constants::PID::Drive::Velocity::Kp,
                                Constants::PID::Drive::Velocity::Kd,
                                Constants::PID::Drive::Velocity::Ki);
    /** The PID used for right motors velocity */
    PID right_velocity_pid = PID(Constants::PID::Drive::Velocity::Kp,
                                 Constants::PID::Drive::Velocity::Kd,
                                 Constants::PID::Drive::Velocity::Ki);
    /** The PID used for turning */
    PID angular_pid = PID(Constants::PID::Drive::Angular::Kp,
                          Constants::PID::Drive::Angular::Kd,
                          Constants::PID::Drive::Angular::Ki);

    /** Type of drive control to use. */
    Constants::DriveType drive_type;

    // Motors:
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> left_front;
    /** Smart pointer to the right, front motor. */
    std::unique_ptr<pros::Motor> right_front;
    /** Smart pointer to the left, back motor. */
    std::unique_ptr<pros::Motor> left_back;
    /** Smart pointer to the right, back motor. */
    std::unique_ptr<pros::Motor> right_back;

    // Motor groups:
    /** Group of all motors on the left side of the robot. */
    std::unique_ptr<pros::MotorGroup> left_motors;
    /** Group of all motors on the right side of the robot. */
    std::unique_ptr<pros::MotorGroup> right_motors;

    /**
     * Helper method to convert RPM to inches per second.
     * @param rpm The RPM to convert to inches per second.
     * @return The RPM converted to inches per second.
     */
    static inline double rpm_to_ips(double rpm);

protected:
    /**
     * Constructor for Drivetrain subsystem.
     * Takes no arguments.
     * Can be called as: <code>Drivetrain drivetrain = Drivetrain();</code>
     */
    Drivetrain();
};

