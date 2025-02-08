#pragma once
#include <cstdint>

namespace Constants {
    namespace Controller {
        /** Constants for the speed used when motors are activated using controller */
        namespace MotorSpeeds {
          
            /** Lift speed for the lift moving upwards */
            constexpr int32_t LIFT_UP = 102;
            /** Lift speed for the lift moving backwards */
            constexpr int32_t LIFT_DOWN = -44;
            /** Lift speed used for resetting, it is slower so that the lift sensor can pick up a reading. */
            constexpr int32_t LIFT_RESET = 25;

            constexpr double LIFT_LADYBROWN_SPEED_MULTIPLIER = 0.7;
          
            /** Motor speed of the intake going inwards */
            constexpr int32_t INTAKE_INWARDS = 127;
            /** Motor speed of the intake going outwards */
            constexpr int32_t INTAKE_OUTWARDS = -44;
        }

        /** The scaling factor used for scaling the controller input */
        constexpr int32_t INPUT_SCALING_FACTOR = 15;
    }
  
    /**
     * Namespace for PID constants used to set drive train velocity.
     * # TODO: Tune
    */
    namespace PID::Drive::Velocity {
        /** PID Proportional coefficient for position calculations */
        constexpr double Kp = 0.0;
        /** PID Derivative coefficient for position calculations */
        constexpr double Kd = 0.0;
        /** PID Integral coefficient for position calculations */
        constexpr double Ki = 0.0;
    }

    /**
     * Namespace for PID constants used to set drive train angular velocity.
     * # TODO: Tune
    */
    namespace PID::Drive::Angular {
        /** PID Proportional coefficient for position calculations */
        constexpr double Kp = 0.0;
        /** PID Derivative coefficient for position calculations */
        constexpr double Kd = 0.0;
        /** PID Integral coefficient for position calculations */
        constexpr double Ki = 0.0;
    }

    /**
     * Namespace for PID constants used to move the drivetrain some number of inches.
     * # TODO: Tune
     */
    namespace PID::Drive::Distance {
        /** PID Proportional coefficient for position calculations */
        constexpr double Kp = 0.0;
        /** PID Derivative coefficient for position calculations */
        constexpr double Kd = 0.0;
        /** PID Integral coefficient for position calculations */
        constexpr double Ki = 0.0;
    }

    /**
     * Namespace for PID constants used to move the drivetrain some number of degrees.
     * # TODO: Tune
    */
    namespace PID::Drive::Degrees {
        /** PID Proportional coefficient for position calculations */
        constexpr double Kp = 0.0;
        /** PID Derivative coefficient for position calculations */
        constexpr double Kd = 0.0;
        /** PID Integral coefficient for position calculations */
        constexpr double Ki = 0.0;
    }

    /**
     * Namespace for PID containing constants used to the position of the LadyBrown (LB).
     */
    namespace PID::LadyBrown::Position {
        /** PID Proportional coefficient for position calculations */
        constexpr double Kp = 2.4;
        /** PID Derivative coefficient for position calculations */
        constexpr double Kd = 0.0;
        /** PID Integral coefficient for position calculations */
        constexpr double Ki = 0.1;
    }
  
    /**
     * This namespace is to be used for the starting configuration of the robot.
     * Specifically, this namespace is for the initial pose of the robot.
    */
    namespace Initial::Pose {
        /** The initial x position of the robot */
        constexpr double INITIAL_X = 0;
        /** The initial y position of the robot */
        constexpr double INITIAL_Y = 0;
        /** The initial heading of the robot */
        constexpr double INITIAL_HEADING = 0;
    }
}