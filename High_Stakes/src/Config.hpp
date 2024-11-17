#pragma once
#include <cstdint>

#include "Command.hpp"
#include "commands/StartIntaking.hpp"


namespace Constants {
    namespace Controller {
        /** Constants for the speed used when motors are activated using controller */
        namespace MotorSpeeds {
          
            /** Lift speed for the lift moving upwards */
            constexpr int32_t LIFT_UP = 30;
            /** Lift speed for the lift moving backwards */
            constexpr int32_t LIFT_DOWN = -30;
          
            /** Motor speed of the intake going inwards */
            constexpr int32_t INTAKE_INWARDS = 70;
            /** Motor speed of the intake going outwards */
            constexpr int32_t INTAKE_OUTWARDS = -70;
        }

        /** Keybindings */
        const std::unordered_map<pros::controller_digital_e_t, std::optional<std::function<std::unique_ptr<Command>()>>> BINDS{
            {pros::E_CONTROLLER_DIGITAL_L1, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_L2, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_R1, []() { return std::make_unique<StartIntaking>(); }}},
            {pros::E_CONTROLLER_DIGITAL_R2, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_UP, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_DOWN, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_LEFT, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_RIGHT, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_X, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_B, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_Y, std::nullopt},
            {pros::E_CONTROLLER_DIGITAL_A, std::nullopt},
            };
    }
}