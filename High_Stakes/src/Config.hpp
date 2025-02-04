#pragma once
#include <cstdint>

#include "Command.hpp"
#include "commands/Default/ResetLift.hpp"
#include "commands/Default/TurnAround.hpp"
#include "commands/Instant/StartClamping.hpp"
#include "commands/Instant/StartIntakingIn.hpp"
#include "commands/Instant/StartIntakingOut.hpp"
#include "commands/Instant/StartLiftingDown.hpp"
#include "commands/Instant/StartLiftingUp.hpp"
#include "commands/Instant/StopClamping.hpp"
#include "commands/Instant/StopIntaking.hpp"


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
          
            /** Motor speed of the intake going inwards */
            constexpr int32_t INTAKE_INWARDS = 127;
            /** Motor speed of the intake going outwards */
            constexpr int32_t INTAKE_OUTWARDS = -44;
        }

        /** The scaling factor used for scaling the controller input */
        constexpr int32_t INPUT_SCALING_FACTOR = 50;

        /** Keybindings. They are mapped like: button -> [button_just_pressed_event, button_down_event,
         * button_just_released_event].
         */
        const std::unordered_map<pros::controller_digital_e_t,
        std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS{
            {pros::E_CONTROLLER_DIGITAL_L1, {std::nullopt, [] { return std::make_unique<StartLiftingDown>(); }, [] { return std::make_unique<ResetLift>(); }}},
            {pros::E_CONTROLLER_DIGITAL_L2, {std::nullopt, [] { return std::make_unique<StartLiftingUp>(); }, [] { return std::make_unique<ResetLift>(); }}},
            {pros::E_CONTROLLER_DIGITAL_R1, {std::nullopt, [] { return std::make_unique<StartIntakingOut>(); }, [] { return std::make_unique<StopIntaking>(); }}},
            {pros::E_CONTROLLER_DIGITAL_R2, {std::nullopt, [] { return std::make_unique<StartIntakingIn>(); }, [] { return std::make_unique<StopIntaking>(); }}},
            {pros::E_CONTROLLER_DIGITAL_UP, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_DOWN, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_LEFT, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_RIGHT, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_X, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_B, {[] { return std::make_unique<TurnAround>(); }, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_Y, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_A, {[] { return std::make_unique<StartClamping>(); }, std::nullopt, [] { return std::make_unique<StopClamping>(); }}}
        };
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