#include "ButtonMappings.hpp"

// Definition of BINDS
#if THINK
// Chris's bindings
std::unordered_map<pros::controller_digital_e_t, std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS {
             {pros::E_CONTROLLER_DIGITAL_L1, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_L2, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_R1, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_R2, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_UP, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_DOWN, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_LEFT, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_RIGHT, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_X, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_B, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_Y, {std::nullopt, std::nullopt, std::nullopt}},
             {pros::E_CONTROLLER_DIGITAL_A, {std::nullopt, std::nullopt, std::nullopt}}
};
#elif DO
// Garret's bindings
std::unordered_map<pros::controller_digital_e_t, std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS {
         {pros::E_CONTROLLER_DIGITAL_L1, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_L2, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_R1, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_R2, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_UP, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_DOWN, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_LEFT, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_RIGHT, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_X, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_B, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_Y, {std::nullopt, std::nullopt, std::nullopt}},
         {pros::E_CONTROLLER_DIGITAL_A, {std::nullopt, std::nullopt, std::nullopt}}
};
#endif
