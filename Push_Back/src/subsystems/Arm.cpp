#include "Arm.hpp"



Arm::Arm() {
    
}

void Arm::initialize() {
    enabled = false;
}

void Arm::periodic() {

}

void Arm::disabled_periodic() {

}

void Arm::shutdown() {

}

void Arm::set_enabled(const bool enabled){
    arm_piston.set_value(enabled);
    this->enabled = enabled;
}

void Arm::toggle(){
    enabled = !enabled;
    arm_piston.set_value(enabled);
}