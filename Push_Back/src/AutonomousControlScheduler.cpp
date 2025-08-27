#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/DriveStraight.hpp"

// Global paths defined here
ASSET(rushCenterFromLeft_txt)
ASSET(alignForGoal_txt)
ASSET(getGoal_txt)
ASSET(getGoal2_txt)
ASSET(intake1_txt)
ASSET(intakeCorner_txt)
ASSET(intake1Do_txt)
ASSET(intake2Do_txt)
ASSET(getGoalDo_txt)
ASSET(doinkTopRingDo_txt)
ASSET(intake1SimpleDo_txt)
ASSET(getGoalSimpleDo_txt)
ASSET(intake2SimpleDo_txt)
ASSET(intake3SimpleDo_txt)
ASSET(touchStakeSimpleDo_txt)

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    // Add commands to the chain here
    #if THINK
    // Thinks autonomous routine goes here:

    #elif DO
    // Dos autonomous routine goes here:

    #endif
}

void AutonomousControlScheduler::initialize() {
    // drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
