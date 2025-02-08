#include "Clamp.hpp"

#include "../ports.hpp"

Clamp::Clamp() {

}

void Clamp::initialize() {
    clamp_piston = std::make_unique<pros::adi::DigitalOut>(Ports::CLAMP_BRAIN_PORT);
    enabled = false;
}

void Clamp::periodic() {

}

void Clamp::disabled_periodic() {

}

void Clamp::shutdown() {

}

void Clamp::set_enabled(const bool enabled) {
    clamp_piston->set_value(enabled);
    this->enabled = enabled;
}

void Clamp::toggle() {
    enabled = !enabled;
    clamp_piston->set_value(enabled);
}