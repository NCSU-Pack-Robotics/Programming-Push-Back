#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/PPIntakeLift.hpp"
 #include "commands/PurePursuit.hpp"
#include "commands/Default/MoveLadyBrownPosition.hpp"
#include "commands/Default/TurnAround.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/Lift.hpp"
#include "subystems/LadyBrown.hpp"
#include "commands/DriveStraight.hpp"

// Global paths defined here
ASSET(getGoal_txt);
ASSET(getFirstRing_txt);
ASSET(touchPost_txt);
ASSET(getGoalThink_txt);
ASSET(getFirstRingThink_txt);
ASSET(touchPostThink_txt);

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

    // Add commands to the chain here
    #if THINK
    add_command_and(make_unique<DriveStraight>(36, 12));  // Go to clib bar
    #elif DO

    #endif
}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
