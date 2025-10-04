#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/DriveStraightProgress.hpp"

// Global paths defined here
// Ex. ASSET(asset_name_txt)

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    // Add commands to the chain here
    #if THINK
    // Thinks autonomous routine goes here:
    add_command(std::make_unique<DriveStraightProgress>(36,0));

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
