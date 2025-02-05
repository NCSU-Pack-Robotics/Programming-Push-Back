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
    // /** Port of the left front drive motor */
    // constexpr std::int8_t LEFT_FRONT_MOTOR_PORT = -11;
    // /** Port of the left back drive motor */
    // constexpr std::int8_t LEFT_BACK_MOTOR_PORT = -19;
    // /** Port of the left middle drive motor */
    // constexpr std::int8_t LEFT_MIDDLE_MOTOR_PORT = ;
    // /** Port of the right front drive motor */
    // constexpr std::int8_t RIGHT_FRONT_MOTOR_PORT = 2;
    // /** Port of the right back drive motor */
    // constexpr std::int8_t RIGHT_BACK_MOTOR_PORT = 21;
    // /** Port of the right middle drive motor */
    // constexpr std::int8_t RIGHT_MIDDLE_MOTOR_PORT = ;

    // These ports are for the competition robot



    /** DO */ 

    // /** Port of the left first front motor */
    // constexpr std::int8_t LEFT_FRONT1_MOTOR_PORT = 12;
    // /** Port of the left second front motor */
    // constexpr std::int8_t LEFT_FRONT2_MOTOR_PORT = -11;
    // /** Port of the left second back motor */
    // constexpr std::int8_t LEFT_BACK2_MOTOR_PORT = 13;
    // /** Port of the left first back motor */
    // constexpr std::int8_t LEFT_BACK1_MOTOR_PORT = -14;

    // /** Port of the right first front motor */
    // constexpr std::int8_t RIGHT_FRONT1_MOTOR_PORT = -16;
    // /** Port of the right second front motor */
    // constexpr std::int8_t RIGHT_FRONT2_MOTOR_PORT = 17;
    // /** Port of the right second back motor */
    // constexpr std::int8_t RIGHT_BACK2_MOTOR_PORT = -18;
    // /** Port of the right first back motor */
    // constexpr std::int8_t RIGHT_BACK1_MOTOR_PORT = 19;

    // /** Port of the lift motor */
    // constexpr std::int8_t LIFT_MOTOR_PORT = 15;

    // /** Port of the intake motor */
    // constexpr std::int8_t INTAKE_MOTOR_PORT = -2;

    // /** Port of the ladybrown motor */
    // constexpr std::int8_t LADYBROWN_MOTOR_PORT = -1;
    // /** Port of the ladybrown sensor motor */
    // constexpr std::int8_t LADYBROWN_SENSOR_MOTOR_PORT = 20;

    // /** Port of the clamp. */
    // constexpr char CLAMP_BRAIN_PORT = 'B';

    // /** Port of the hook sensor, LOW value means the sensor is blocked. */
    // constexpr char HOOK_SENSOR_BRAIN_PORT = 'A';



    /** THINK */
    /** Port of the left first front motor */
    constexpr std::int8_t LEFT_FRONT1_MOTOR_PORT = 15;
    /** Port of the left second front motor */
    constexpr std::int8_t LEFT_FRONT2_MOTOR_PORT = -16;
    /** Port of the left second back motor */
    constexpr std::int8_t LEFT_BACK2_MOTOR_PORT = 17;
    /** Port of the left first back motor */
    constexpr std::int8_t LEFT_BACK1_MOTOR_PORT = -18;

    /** Port of the right first front motor */
    constexpr std::int8_t RIGHT_FRONT1_MOTOR_PORT = -14;
    /** Port of the right second front motor */
    constexpr std::int8_t RIGHT_FRONT2_MOTOR_PORT = 13;
    /** Port of the right second back motor */
    constexpr std::int8_t RIGHT_BACK2_MOTOR_PORT = -12;
    /** Port of the right first back motor */
    constexpr std::int8_t RIGHT_BACK1_MOTOR_PORT = 11;

    /** Port of the lift motor */
    constexpr std::int8_t LIFT_MOTOR_PORT = 9;

    /** Port of the intake motor */
    constexpr std::int8_t INTAKE_MOTOR_PORT = -10;

    /** Port of the ladybrown motor */
    constexpr std::int8_t LADYBROWN_MOTOR_PORT = -1;
    /** Port of the ladybrown sensor motor */
    constexpr std::int8_t LADYBROWN_ROTATION_PORT = 20;

    /** Port of the clamp. */
    constexpr char CLAMP_BRAIN_PORT = 'A';

    /** Port of the hook sensor, LOW value means the sensor is blocked. */
    constexpr char HOOK_SENSOR_BRAIN_PORT = 'B';

    /** Port of the inertial sensor (gyro) */
    // constexpr std::int8_t INERTIAL_SENSOR_PORT = 1;
}
