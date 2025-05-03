#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/Default/MoveLadyBrownPosition.hpp"
#include "commands/Default/TurnAround.hpp"
#include "commands/DriveStraight.hpp"
#include "commands/Instant/SetReversed.hpp"
#include "commands/Instant/ToggleArm.hpp"
#include "commands/PPIntakeLift.hpp"
#include "commands/PurePursuit.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/LadyBrown.hpp"
#include "subystems/Lift.hpp"

// Global paths defined here
ASSET(rushCenterFromLeft_txt)

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

    // Add commands to the chain here
    #if THINK
    add_command_and(make_unique<DriveStraight>(42, 1.5)).
    add_command_and(make_unique<ToggleArm>()).
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(250);}))).
    add_command_and(make_unique<DriveStraight>(-35, 5)).
    add_command_and(make_unique<ToggleArm>()).
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(350);}))).
    add_command_and(make_unique<PurePursuit>(rushCenterFromLeft_txt)).
    add_command_and(make_unique<ToggleArm>()).
    add_command_and(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(250);}))).
    add_command_and(make_unique<DriveStraight>(-20, 5)).
    add_command_and(make_unique<ToggleArm>());


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


    #endif
}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
