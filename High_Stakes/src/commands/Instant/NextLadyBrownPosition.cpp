#include "NextLadyBrownPosition.hpp"
#include "../../subystems/LadyBrown.hpp"

NextLadyBrownPosition::NextLadyBrownPosition() {

}

void NextLadyBrownPosition::execute() {
    LadyBrown& ladyBrown = AbstractSubsystem::get_instance<LadyBrown>();

    ladyBrown.next_position();
}