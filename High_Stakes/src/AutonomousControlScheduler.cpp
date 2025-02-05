#include "AutonomousControlScheduler.hpp"

#include "commands/PurePursuit.hpp"
#include "asset.hpp"

// Global paths defined here
ASSET(loop1_txt);
ASSET(loop2_txt);

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    // Add commands to the chain here
    add_command_and(std::make_unique<PurePursuit>(loop1_txt)).
        add_command(std::make_unique<PurePursuit>(loop2_txt));
}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.brake();
}
