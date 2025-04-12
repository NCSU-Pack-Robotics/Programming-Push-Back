#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/DriveStraight.hpp"
#include "commands/PPIntakeLift.hpp"
 #include "commands/PurePursuit.hpp"
#include "commands/Default/MoveLadyBrownPosition.hpp"
#include "commands/Default/TurnAround.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/Lift.hpp"
#include "subystems/LadyBrown.hpp"

// Global paths defined here
//ASSET(getGoal_txt);
//ASSET(getFirstRing_txt);
//ASSET(touchPost_txt);
//ASSET(getGoalThink_txt);
//ASSET(getFirstRingThink_txt);
//ASSET(touchPostThink_txt);

ASSET(pathPart0_txt);
ASSET(pathPart1_txt);
ASSET(pathPart2_txt);
ASSET(pathPart3_txt);
ASSET(pathPart4_txt);


using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

    // Add commands to the chain here
    #if THINK
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
    #elif DO
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(true); }))).
    add_command_and(make_unique<PurePursuit>(pathPart0_txt)).
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { clamp.set_enabled(true); })));
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(false); }))).
    add_command_and(make_unique<PPIntakeLift>(pathPart1_txt)).  // Follow path, intake, and lift ring
    add_command_and(make_unique<PPIntakeLift>(pathPart2_txt)).
    add_command_and(make_unique<PPIntakeLift>(pathPart3_txt)).
    add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { drivetrain.set_reversing(true); }))).
    add_command_and(make_unique<PurePursuit>(pathPart4_txt));
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
        [&] { clamp.set_enabled(false); })));

    // add_command_and(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
    //     [&] { drivetrain.set_reversing(true); }))).  // Reverse the drivetrain
    // add_command_and(make_unique<PurePursuit>(getGoal_txt)).  // Follow path to goal
    // add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
    //     [&] { clamp.set_enabled(true); }))); // Enable the clamp
    // add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
    //     [&] { pros::delay(1000); }))).  // Pause for a second
    // add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
    //     [&] { drivetrain.set_reversing(false); }))).  // Stop reversing the drivetrain
    // add_command_and(make_unique<PPIntakeLift>(getFirstRing_txt)).  // Follow path, intake, and lift ring
    // add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
    //     [&] { pros::delay(2500); }))).  // Pause for a second
    // add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
    //     [&] { lift.brake(); intake.brake(); }))).  // Stop intake and lift
    // add_command_and(make_unique<MoveLadyBrownPosition>(LadyBrown::Position::SCORE)).  // Raise LadyBrown
    // add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>(
    //     [&] { lady_brown.set_killed(true); }))).  // Stop intake and lift
    // add_command_and(make_unique<TurnAround>()).
    // add_command_and(make_unique<PurePursuit>(touchPost_txt));  // Go to clib bar
    #endif
}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
