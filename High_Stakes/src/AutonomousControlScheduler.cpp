#include "AutonomousControlScheduler.hpp"

#include "commands/PurePursuit.hpp"
#include "asset.hpp"
#include "commands/Default/MoveLiftDegrees.hpp"
#include "subystems/Lift.hpp"
#include "subystems/Clamp.hpp"

// Global paths defined here
ASSET(getGoal_txt);
ASSET(getFirstRing_txt);

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    // Add commands to the chain here
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(true); }))).
    add_command_and(make_unique<PurePursuit>(getGoal_txt)).
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { clamp.set_enabled(true); }))).
    add_command_and(make_unique<MoveLiftDegrees>(1600)).
    add_command_and(make_unique<PurePursuit>(getFirstRing_txt)).
    add_command_and(make_unique<MoveLiftDegrees>(1600));

}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.brake();
}
