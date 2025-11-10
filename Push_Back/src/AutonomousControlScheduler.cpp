#include "AutonomousControlScheduler.hpp"


#include "asset.hpp"
#include "Command.hpp"
#include "commands/DriveStraight.hpp"

// Global paths defined here
// Ex. ASSET(asset_name_txt)

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    // Add commands to the chain here
    #if THINK
    // Thinks autonomous routine goes here:

    // for testing
    add_command(make_unique<TimelineCommand>(make_unique<DriveStraight>(12, 2),
    std::initializer_list<TimelineCommand::Checkpoint>{
    {1.0, std::make_unique<DriveStraight>(1.0, 1.0)}
    }));

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
