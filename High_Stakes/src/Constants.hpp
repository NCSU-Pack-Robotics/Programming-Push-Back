#pragma once

/** 90 degrees */
#define HALF_PI (1.57079632679489661923)

/** 180 degrees */
#define PI (3.14159265358979323846)

/** 360 degrees */
# define TAU (6.28318530717958647692)

/**
 * The diameter of a tracking wheel
 *     • Measured between outer omni rollers
 *     • Measured in inches
*/
#define TRACKING_DIAMETER (3.25)

/** Ratio of tracking wheel upper gear to lower wheel gear */
#define TRACKING_RATIO (3.0 / 5.0)

/**
 * Distance between the centers of each tracking wheel
 *      • Measured in inches
*/
#define ROBOT_DIAMETER (12.5)

/**
 * Half the distance between the centers of each tracking wheel
 *      • Measured in inches
*/
#define ROBOT_RADIUS (ROBOT_DIAMETER/2)

/** Maximum motor voltage in milli-volts */
#define MAX_MOTOR_VOLTAGE (12'000) // 12 volts

/** Initial x-coordinate of the robot */
constexpr double INITIAL_X = 0.0;

/** Initial y-coordinate of the robot */
constexpr double INITIAL_Y = 0.0;

/** Initial heading of the robot */
constexpr double INITIAL_HEADING = 0.0;

