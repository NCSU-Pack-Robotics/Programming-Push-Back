#include "AbstractOdometry.hpp"

AbstractOdometry::AbstractOdometry(const Pose& initial_pose)
 : pose(initial_pose) {
}


Pose AbstractOdometry::get_pose() const {
    return this->pose;
}

void AbstractOdometry::set_pose(const Pose new_pose) {
    this->pose = new_pose;
}
