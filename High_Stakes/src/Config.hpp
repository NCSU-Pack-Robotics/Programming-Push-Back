#pragma once
#include <cstdint>

#include "Command.hpp"
#include "commands/StartIntaking.hpp"
#include "commands/StopIntaking.hpp"


namespace Constants {
    namespace Controller {
        /** Constants for the speed used when motors are activated using controller */
        namespace MotorSpeeds {
          
            /** Lift speed for the lift moving upwards */
            constexpr int32_t LIFT_UP = 30;
            /** Lift speed for the lift moving backwards */
            constexpr int32_t LIFT_DOWN = -30;
          
            /** Motor speed of the intake going inwards */
            // TODO: Set back to 70, 10 is for testing purposes.
            constexpr int32_t INTAKE_INWARDS = 10;
            /** Motor speed of the intake going outwards */
            constexpr int32_t INTAKE_OUTWARDS = -70;
        }


        /** Keybindings. They are mapped like: button -> [button_just_pressed_event, button_down_event,
         * button_just_released_event].
         */
        const std::unordered_map<pros::controller_digital_e_t,
        std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS{
            {pros::E_CONTROLLER_DIGITAL_L1, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_L2, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_R1, {[] { return std::make_unique<StartIntaking>(); }, std::nullopt, [] { return std::make_unique<StopIntaking>(); }}},
            {pros::E_CONTROLLER_DIGITAL_R2, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_UP, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_DOWN, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_LEFT, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_RIGHT, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_X, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_B, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_Y, {std::nullopt, std::nullopt, std::nullopt}},
            {pros::E_CONTROLLER_DIGITAL_A, {std::nullopt, std::nullopt, std::nullopt}},
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