#include "PurePursuit.hpp"

PurePursuit::PurePursuit(Pose start, Pose end) : start(start), end(end) {
    this->current = drivetrain.get_pose();
}

void PurePursuit::initialize() {
}

void PurePursuit::periodic() {
}

void PurePursuit::shutdown() {
}

bool PurePursuit::is_complete() {
    return false;
}
