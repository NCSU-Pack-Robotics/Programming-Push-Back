#include "NextLadyBrownPosition.hpp"
#include "../../subystems/LadyBrown.hpp"

NextLadyBrownPosition::NextLadyBrownPosition() {

}

void NextLadyBrownPosition::execute() {
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();
    
    lady_brown.next_position();
}