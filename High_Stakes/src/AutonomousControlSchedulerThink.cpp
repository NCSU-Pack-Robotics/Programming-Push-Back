#include "AutonomousControlSchedulerThink.hpp"

#include "commands/PPIntakeLift.hpp"
#include "asset.hpp"
#include "subystems/LadyBrown.hpp"
#include "commands/PurePursuit.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/Lift.hpp"

// Global paths defined here
ASSET(getGoalThink_txt);
ASSET(getFirstRingThink_txt);
ASSET(touchPostThink_txt);

using namespace std;

AutonomousControlSchedulerThink::AutonomousControlSchedulerThink(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

    // Add commands to the chain here
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(true); }))).  // Reverse the drivetrain
    add_command_and(make_unique<PurePursuit>(getGoalThink_txt)).  // Follow path to goal
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { clamp.set_enabled(true); }))); // Enable the clamp
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { pros::delay(1000); }))).  // Pause for a second
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { drivetrain.set_reversing(false); }))).  // Stop reversing the drivetrain
    add_command_and(make_unique<PPIntakeLift>(getFirstRingThink_txt)).  // Follow path, intake, and lift ring
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { pros::delay(2500); }))).  // Pause for a second
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { lift.brake(); intake.brake(); }))).  // Stop intake and lift
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { lady_brown.set_position(LadyBrown::Position::SCORE); }))).  // Raise LadyBrown
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { pros::delay(1000); }))).  // Pause for a second
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { lady_brown.set_killed(true); }))).  // Stop intake and lift
    add_command_and(make_unique<PurePursuit>(touchPostThink_txt));  // Go to clib bar

}

void AutonomousControlSchedulerThink::initialize() {
    drivetrain.initialize();
}

void AutonomousControlSchedulerThink::shutdown() {
    drivetrain.brake();
}
