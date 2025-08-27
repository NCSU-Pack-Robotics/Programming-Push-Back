#include "TurnDegrees.hpp"

TurnDegrees::TurnDegrees(double degrees, double tolerance, double max_time) :
    TurnToHeading(degrees, tolerance * M_PI / 180.0, max_time) {  // Convert degrees to radians
    this->to_turn = degrees * M_PI / 180.0;  // Convert degrees to radians
}

void TurnDegrees::initialize() {
    const double current_heading = drivetrain.get_pose().heading;

    this->target_heading = current_heading + this->to_turn;

    // Call the base class initialize method
    TurnToHeading::initialize();
}

