#include "Clamp.hpp"
Clamp::Clamp() {

}

void Clamp::initialize() {
    enabled = false;
}

void Clamp::periodic() {

}

void Clamp::disabled_periodic() {

}

void Clamp::shutdown() {

}

void Clamp::set_enabled(const bool enabled) {
    clamp_piston.set_value(enabled);
    this->enabled = enabled;
}

void Clamp::toggle() {
    enabled = !enabled;
    clamp_piston.set_value(enabled);
}