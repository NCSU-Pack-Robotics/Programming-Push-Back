#pragma once

#include <algorithm>
#include <unordered_map>
#include "Command.hpp"
#include "commands/Default/ResetLift.hpp"
#include "commands/Default/TurnAround.hpp"
#include "commands/Instant/NextLadyBrownPosition.hpp"
#include "commands/Instant/StartIntakingIn.hpp"
#include "commands/Instant/StartIntakingOut.hpp"
#include "commands/Instant/StartLiftingDown.hpp"
#include "commands/Instant/StartLiftingUp.hpp"
#include "commands/Instant/StopIntaking.hpp"
#include "commands/Instant/StopLifting.hpp"
#include "commands/Instant/ToggleArm.hpp"
#include "commands/Instant/ToggleClamp.hpp"

// Magic hash number
#define GOLDEN_RATIO 0x9e3779b1

struct ButtonCombo {
    std::vector<pros::controller_digital_e_t> buttons;

    bool operator==(const ButtonCombo& other) const {
        if (buttons.size() != other.buttons.size()) return false;
        std::vector<pros::controller_digital_e_t> sorted_buttons = buttons;
        std::vector<pros::controller_digital_e_t> sorted_other = other.buttons;
        std::sort(sorted_buttons.begin(), sorted_buttons.end());
        std::sort(sorted_other.begin(), sorted_other.end());
        return sorted_buttons == sorted_other;
    }
};

// Needed so we can use it in an unordered_map
namespace std {
    template <>
    struct hash<ButtonCombo> {
        std::size_t operator()(const ButtonCombo& combo) const {
            std::size_t seed = 0;
            for (const pros::controller_digital_e_t& btn : combo.buttons) {
                printf("Hashing button %d" + btn);
                seed ^= std::hash<int>()(btn) + GOLDEN_RATIO + (seed << 6) + (seed >> 2);
            }
            printf("returning seed: %d\n", seed);
            return seed;
        }
    };
}

/** Keybindings. They are mapped like: button -> [button_just_pressed_event, button_down_event,
 * button_just_released_event].
 */
// If you need to map a ButtonCombo to multiple commands, use an inline parallel or chain command.

// CHRIS DRIVE CONFIG

const std::unordered_map<ButtonCombo,
                         std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS{
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_L1}}, {std::nullopt, [] { return std::make_unique<StartLiftingDown>(); }, [] { return std::make_unique<StopLifting>(); }}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_L2}}, {std::nullopt, [] { return std::make_unique<StartLiftingUp>(); }, [] { return std::make_unique<StopLifting>(); }}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_R1}}, {std::nullopt, [] { return std::make_unique<StartIntakingOut>(); }, [] { return std::make_unique<StopIntaking>(); }}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_R2}}, {std::nullopt, [] { return std::make_unique<StartIntakingIn>(); }, [] { return std::make_unique<StopIntaking>(); }}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_UP}}, {std::nullopt, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_DOWN}}, {std::nullopt, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_LEFT}}, {std::nullopt, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_RIGHT}}, {std::nullopt, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_X}}, {[] { return std::make_unique<ToggleArm>(); }, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_B}}, {[] { return std::make_unique<TurnAround>(); }, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_Y}}, {[] { return std::make_unique<NextLadyBrownPosition>(); }, std::nullopt, std::nullopt}},
                             {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_A}}, {[] { return std::make_unique<ToggleClamp>(); }, std::nullopt, std::nullopt}},
};

// GARETT DRIVE CONFIG
// const std::unordered_map<ButtonCombo,
//                          std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS{
//                              {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_L2, pros::E_CONTROLLER_DIGITAL_R2}}, {[] { return std::make_unique<StartIntakingOut>(); }, std::nullopt, std::nullopt}},
//                                 {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_R2}}, {std::nullopt, std::nullopt, [] { return std::make_unique<StopIntaking>(); }}},
//                              {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_L2, pros::E_CONTROLLER_DIGITAL_R1}}, {[] { return std::make_unique<StartLiftingDown>(); }, std::nullopt, std::nullopt}},
//                              {ButtonCombo{std::vector{pros::E_CONTROLLER_DIGITAL_R1}}, {std::nullopt, std::nullopt, [] { return std::make_unique<StopLifting>(); }}},
//                              // {ButtonCombo{std::vector{pros::E_CONTROLLER}}} // {ButtonCombo{std::vector{pros::E_CONTROLLER}}} // How to do a command if R2 is pressed and L2 is NOT pressed?
//
//
// };