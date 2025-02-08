#include "Arm.hpp"

#include "../ports.hpp"

Arm::Arm() {

}

void Arm::initialize() {
    arm_piston = std::make_unique<pros::adi::DigitalOut>(Ports::ARM_BRAIN_PORT);
    enabled = false;
}

void Arm::periodic() {

}

void Arm::disabled_periodic() {

}

void Arm::shutdown() {

}

void Arm::set_enabled(const bool enabled){
    arm_piston->set_value(enabled);
    this->enabled = enabled;
}

void Arm::toggle(){
    enabled = !enabled;
    arm_piston->set_value(enabled);   
}