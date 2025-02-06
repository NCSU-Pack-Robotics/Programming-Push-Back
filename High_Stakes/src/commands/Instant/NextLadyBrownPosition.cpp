#include "NextLadyBrownPosition.hpp"
#include "../../subystems/LadyBrown.hpp"

NextLadyBrownPosition::NextLadyBrownPosition() {

}

void NextLadyBrownPosition::execute() {
    LadyBrown& ladyBrown = AbstractSubsystem::get_instance<LadyBrown>();

    printf("NextLadyBrownPosition\n");
    ladyBrown.next_position();
}