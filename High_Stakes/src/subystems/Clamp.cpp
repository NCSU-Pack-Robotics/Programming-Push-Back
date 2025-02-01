#include "Clamp.hpp"

#include "../ports.hpp"
#include "pros/adi.hpp"

Clamp::Clamp() {

}

void Clamp::initialize() {
    clamp_piston = std::make_unique<pros::ADIDigitalOut>(Ports::CLAMP_BRAIN_PORT);
}

void Clamp::periodic() {

}

void Clamp::disabled_periodic() {

}

void Clamp::shutdown() {

}

void Clamp::set_enabled(bool enabled) {
    clamp_piston->set_value(HIGH);
}






