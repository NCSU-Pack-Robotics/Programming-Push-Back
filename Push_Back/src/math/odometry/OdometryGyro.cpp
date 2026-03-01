#include "OdometryGyro.hpp"

#include "Config.hpp"

OdometryGyro::OdometryGyro(const Pose& initial_pose)
    : OdometryArc(initial_pose) {

    this->injected_heading = initial_pose.heading;
    this->last_injected_heading = this->injected_heading;
}

Pose OdometryGyro::calculate(const std::pair<double, double> &positions, const double heading) {
    // Updated the injected heading so when calculate calls, the update methods, this will be used
    this->injected_heading = heading;

    const Pose pose = OdometryArc::calculate(positions);

    // Updated after everything
    this->last_injected_heading = heading;

    return pose;
}

void OdometryGyro::update_heading() {
    const double initial_heading = this->pose_updated ? this->reset_heading : Constants::Initial::Pose::INITIAL_HEADING;
    this->pose.heading = initial_heading + this->injected_heading;
}

void OdometryGyro::update_deltas() {
    OdometryArc::update_deltas();

    this->delta_heading = this->injected_heading - this->last_injected_heading;
}
