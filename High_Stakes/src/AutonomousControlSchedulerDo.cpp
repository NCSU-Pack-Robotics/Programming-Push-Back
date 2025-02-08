#include "AutonomousControlSchedulerDo.hpp"

#include "asset.hpp"
#include "commands/PPIntakeLift.hpp"
#include "commands/Default/MoveLiftDegrees.hpp"
#include "commands/PurePursuit.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/Lift.hpp"

// Global paths defined here
ASSET(getGoal_txt);
ASSET(getFirstRing_txt);

using namespace std;

AutonomousControlSchedulerDo::AutonomousControlSchedulerDo(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    // Add commands to the chain here
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(true); }))).  // Reverse the drivetrain
    add_command_and(make_unique<PurePursuit>(getGoal_txt)).  // Follow path to goal
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { clamp.set_enabled(true); }))); // Enable the clamp
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { pros::delay(500); }))).  // Pause for a second
    add_command_and(make_unique<MoveLiftDegrees>(1600)). // Lift the ring
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { drivetrain.set_reversing(false); }))).  // Stop reversing the drivetrain
    add_command_and(make_unique<PPIntakeLift>(getFirstRing_txt));  // Follow path, intake, and lift ring
    // add_command_and(make_unique<MoveLiftDegrees>(1600));
}

void AutonomousControlSchedulerDo::initialize() {
    drivetrain.initialize();
}

void AutonomousControlSchedulerDo::shutdown() {
    drivetrain.brake();
}
