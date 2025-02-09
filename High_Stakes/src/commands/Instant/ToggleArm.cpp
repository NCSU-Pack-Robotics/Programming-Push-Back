#include "ToggleArm.hpp"

#include "../../subystems/Arm.hpp"

ToggleArm::ToggleArm() {
}

void ToggleArm::execute() {
    Arm& arm = AbstractSubsystem::get_instance<Arm>();

    arm.toggle();
}