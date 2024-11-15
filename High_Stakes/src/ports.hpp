#pragma once

#include <cstdint>

/**
 * This namespace contains all the ports for the robot.
 * Motors, sensors, and other devices should be declared here.
 */
namespace Ports {
    //TODO: Change ports to competition robot ports
  
    // Note: negative values reverse the motor's direction

    // These ports are for the non-competition robot
    /** Port of the left front drive motor */
    constexpr std::int8_t LEFT_FRONT_MOTOR_PORT = -11;
    /** Port of the left back drive motor */
    constexpr std::int8_t LEFT_BACK_MOTOR_PORT = -19;
    /** Port of the right front drive motor */
    constexpr std::int8_t RIGHT_FRONT_MOTOR_PORT = 2;
    /** Port of the right back drive motor */
    constexpr std::int8_t RIGHT_BACK_MOTOR_PORT = 21;


    // This port is for the competition robot
    /** Port of the lift motor */
    constexpr std::int8_t LIFT_MOTOR_PORT = 1;
  
    // This port is for the competition robot
    /** Port of the intake motor */
    constexpr std::int8_t INTAKE_MOTOR_PORT = 14;

    /** Port of the inertial sensor (gyro) */
    // constexpr std::int8_t INERTIAL_SENSOR_PORT = 1;
}
