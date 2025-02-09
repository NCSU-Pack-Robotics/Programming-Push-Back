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
                seed ^= std::hash<int>()(btn) + GOLDEN_RATIO + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

/** Keybindings. They are mapped like: button -> [button_just_pressed_event, button_down_event,
 * button_just_released_event].
 */
// If you need to map a ButtonCombo to multiple commands, use an inline parallel or chain command.



// Declaration of BINDS
extern std::unordered_map<pros::controller_digital_e_t,
                          std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS;

class GarretShiftIn : public InstantCommand {
public:

    GarretShiftIn() = default;

    void execute() override {
        BINDS[pros::E_CONTROLLER_DIGITAL_R1] = {std::nullopt, [] { return std::make_unique<StartLiftingDown>(); }, [] { return std::make_unique<StopLifting>(); }};
        BINDS[pros::E_CONTROLLER_DIGITAL_R2] = {std::nullopt, [] { return std::make_unique<StartIntakingOut>(); }, [] { return std::make_unique<StopIntaking>(); }};
    }
};

class GarretShiftOut : public InstantCommand {
public:

    GarretShiftOut() = default;

    void execute() override {
        BINDS[pros::E_CONTROLLER_DIGITAL_R1] = {std::nullopt, [] { return std::make_unique<StartLiftingUp>(); }, [] { return std::make_unique<StopLifting>(); }};
        BINDS[pros::E_CONTROLLER_DIGITAL_R2] = {std::nullopt, [] { return std::make_unique<StartIntakingIn>(); }, [] { return std::make_unique<StopIntaking>(); }};
    }
};
