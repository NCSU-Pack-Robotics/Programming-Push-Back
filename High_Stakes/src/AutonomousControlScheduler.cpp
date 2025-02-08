#include "AutonomousControlScheduler.hpp"

#include "commands/PurePursuit.hpp"
#include "commands/DriveStraight.hpp"
#include "asset.hpp"

// Global paths defined here
ASSET(straight72_txt);
ASSET(back72_txt);

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    // Add commands to the chain here
    // add_command_and(std::make_unique<PurePursuit>(straight72_txt)).
    //     add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>([&] { drivetrain.set_reversing(true); }))).
    //     add_command_and(std::make_unique<PurePursuit>(back72_txt)).
    //     add_command(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>([&] {drivetrain.set_reversing(false); })));
    add_command(std::make_unique<DriveStraight>(72.0, 1));
}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.brake();
}
