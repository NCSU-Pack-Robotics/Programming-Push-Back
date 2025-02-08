#include "AutonomousControlSchedulerThink.hpp"

#include "asset.hpp"
#include "commands/Default/MoveLiftDegrees.hpp"
#include "commands/PurePursuit.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/Lift.hpp"

// Global paths defined here
ASSET(getGoalThink_txt);
ASSET(getFirstRingThink_txt);

using namespace std;

AutonomousControlSchedulerThink::AutonomousControlSchedulerThink(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    // Add commands to the chain here
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(true); }))).
    add_command_and(make_unique<PurePursuit>(getGoalThink_txt)).
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { clamp.set_enabled(true); }))).
    add_command_and(make_unique<MoveLiftDegrees>(1600)).
    add_command_and(make_unique<PurePursuit>(getFirstRingThink_txt)).
    add_command_and(make_unique<MoveLiftDegrees>(1600));

}

void AutonomousControlSchedulerThink::initialize() {
    drivetrain.initialize();
}

void AutonomousControlSchedulerThink::shutdown() {
    drivetrain.brake();
}
