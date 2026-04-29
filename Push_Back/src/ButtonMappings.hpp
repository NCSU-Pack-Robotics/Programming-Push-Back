#pragma once

#include <algorithm>
#include <unordered_map>
#include "architecture/commands/Command.hpp"

// Magic hash number
#define GOLDEN_RATIO 0x9e3779b1

struct ButtonCombo {
    std::vector<pros::controller_digital_e_t> buttons;

    bool operator==(const ButtonCombo& other) const {
        if (buttons.size() != other.buttons.size()) return false;
        std::vector<pros::controller_digital_e_t> sorted_buttons = buttons;
        std::vector<pros::controller_digital_e_t> sorted_other = other.buttons;
        std::ranges::sort(sorted_buttons);
        std::ranges::sort(sorted_other);
        return sorted_buttons == sorted_other;
    }
};

// Needed so we can use it in an unordered_map
template <>
struct std::hash<ButtonCombo> {
    std::size_t operator()(const ButtonCombo& combo) const {
        std::size_t seed = 0;
        for (const pros::controller_digital_e_t& btn : combo.buttons) {
            seed ^= std::hash<int>()(btn) + GOLDEN_RATIO + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

/** Keybindings. They are mapped like: button -> [button_just_pressed_event, button_down_event,
 * button_just_released_event].
 */
// If you need to map a ButtonCombo to multiple commands, use an inline parallel or chain command.
extern std::unordered_map<pros::controller_digital_e_t,
                          std::array<std::optional<std::function<std::unique_ptr<Command>()>>, 3>> BINDS;
