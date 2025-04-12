#include "AbstractOdometry.hpp"

AbstractOdometry::AbstractOdometry(Pose initial_pose)
 : pose(initial_pose) {
}


Pose AbstractOdometry::get_pose() const {
    return this->pose;
}
