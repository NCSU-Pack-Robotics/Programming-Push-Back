#pragma once

/**
 * This namespace contains all the ports for the robot.
 * Motors, sensors, and other devices should be declared here.
 */
namespace Ports {
    #if THINK

    /** Port of the left first front motor */
    constexpr std::int8_t LEFT_FRONT1_MOTOR_PORT = 11;
    /** Port of the left second front motor */
    constexpr std::int8_t LEFT_FRONT2_MOTOR_PORT = -12;
    /** Port of the left second back motor */
    constexpr std::int8_t LEFT_BACK2_MOTOR_PORT = 13;
    /** Port of the left first back motor */
    constexpr std::int8_t LEFT_BACK1_MOTOR_PORT = -14;

    /** Port of the right first front motor */
    constexpr std::int8_t RIGHT_FRONT1_MOTOR_PORT = -2;
    /** Port of the right second front motor */
    constexpr std::int8_t RIGHT_FRONT2_MOTOR_PORT = 3;
    /** Port of the right second back motor */
    constexpr std::int8_t RIGHT_BACK2_MOTOR_PORT = -4;
    /** Port of the right first back motor */
    constexpr std::int8_t RIGHT_BACK1_MOTOR_PORT = 5;

    /** Port of the left rotation sensor */
    constexpr std::int8_t LEFT_ROTATION_SENSOR_PORT = 15;
    /** Port of the right rotation sensor */
    constexpr std::int8_t RIGHT_ROTATION_SENSOR_PORT = 1;

    /** Port of the lift motor */
    constexpr std::int8_t LIFT_MOTOR_PORT = 7;

    /** Port of the intake motor */
    constexpr std::int8_t INTAKE_MOTOR_PORT = -6;

    /** Port of the ladybrown motor */
    constexpr std::int8_t LADYBROWN_MOTOR_PORT = 8;
    /** Port of the ladybrown rotation sensor */
    constexpr std::int8_t LADYBROWN_ROTATION_PORT = 21;

    /** Port of the clamp. */
    constexpr char CLAMP_BRAIN_PORT = 'E';

    /** Port of the hook sensor, LOW value means the sensor is blocked. */
    constexpr char HOOK_SENSOR_BRAIN_PORT = 'B';

    /** Port of the extended arm port */
    constexpr char ARM_BRAIN_PORT = 'A';

    #elif DO

    /** Port of the left first front motor */
    constexpr std::int8_t LEFT_FRONT1_MOTOR_PORT = 13;
    /** Port of the left second front motor */
    constexpr std::int8_t LEFT_FRONT2_MOTOR_PORT = -14;
    /** Port of the left second back motor */
    constexpr std::int8_t LEFT_BACK2_MOTOR_PORT = 12;
    /** Port of the left first back motor */
    constexpr std::int8_t LEFT_BACK1_MOTOR_PORT = -11;

    /** Port of the right first front motor */
    constexpr std::int8_t RIGHT_FRONT1_MOTOR_PORT = -4;
    /** Port of the right second front motor */
    constexpr std::int8_t RIGHT_FRONT2_MOTOR_PORT = 5;
    /** Port of the right second back motor */
    constexpr std::int8_t RIGHT_BACK2_MOTOR_PORT = -6;
    /** Port of the right first back motor */
    constexpr std::int8_t RIGHT_BACK1_MOTOR_PORT = 7;

    /** Port of the left rotation sensor */
    constexpr std::int8_t LEFT_ROTATION_SENSOR_PORT = 2;
    /** Port of the right rotation sensor */
    constexpr std::int8_t RIGHT_ROTATION_SENSOR_PORT = -15;

    /** Port of the lift motor */
    constexpr std::int8_t LIFT_MOTOR_PORT = 8;

    /** Port of the intake motor */
    constexpr std::int8_t INTAKE_MOTOR_PORT = -2;

    /** Port of the ladybrown motor */
    constexpr std::int8_t LADYBROWN_MOTOR_PORT = 9;
    /** Port of the ladybrown sensor motor */
    constexpr std::int8_t LADYBROWN_ROTATION_PORT = 10;

    /** Port of the clamp. */
    constexpr char CLAMP_BRAIN_PORT = 'A';

    /** Port of the hook sensor, LOW value means the sensor is blocked. */
    constexpr char HOOK_SENSOR_BRAIN_PORT = 'H';

    // /** Port of the extended arm port */
    /// Needed to compile, but there is no arm on DO
    constexpr char ARM_BRAIN_PORT = 'B';

    #endif
}
