#include "SetReversed.hpp"

#include "../../subsystems/Drivetrain.hpp"

void SetReversed::execute() {
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    drivetrain.set_reversing(this->reversed);
}

std::string SetReversed::to_string() const {
    std::string result = "SetReversed(";
    if (reversed) {
        result += "true";
    } else {
        result += "false";
    }
    result += ")";
    return result;
}
