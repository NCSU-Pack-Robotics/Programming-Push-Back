#include "MoveLadyBrownPosition.hpp"

MoveLadyBrownPosition::MoveLadyBrownPosition(LadyBrown::Position position) {
    this->position = position;
}

void MoveLadyBrownPosition::initialize() {
    lady_brown.set_position(this->position);
}

void MoveLadyBrownPosition::periodic() {

}

bool MoveLadyBrownPosition::is_complete() {
    return lady_brown.get_last_position() == this->position;
}

void MoveLadyBrownPosition::shutdown() {

}





