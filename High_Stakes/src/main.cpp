#include "../include/main.h"

#include "AutonomousControlScheduler.hpp"
#include "DriverControlScheduler.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/Drivetrain.hpp"
#include "subystems/HookSensor.hpp"
#include "subystems/Intake.hpp"
#include "subystems/LadyBrown.hpp"
#include "subystems/Lift.hpp"

// Create all subsystems:
Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
Lift& lift = AbstractSubsystem::get_instance<Lift>();
Intake& intake = AbstractSubsystem::get_instance<Intake>();
Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
HookSensor& hook_sensor = AbstractSubsystem::get_instance<HookSensor>();
LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

// Add subsystems to vector for iteration
std::vector<AbstractSubsystem*> subsystems = {&drivetrain, &lift, &intake, &clamp, &hook_sensor, &lady_brown};

/** Controller object */
pros::Controller controller{pros::E_CONTROLLER_MASTER};

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    // Initialize all subsystems
    for (AbstractSubsystem* subsystem : subsystems) {
        subsystem->initialize();
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    // Ensure all subsystems have shutdown/stopped.
    for (AbstractSubsystem* subsystem : subsystems) {
        subsystem->shutdown();
    }

    // Run disabled periodic for all subsystems
    while (true) {
        for (AbstractSubsystem* subsystem: subsystems) {
            subsystem->disabled_periodic();
        }
    }
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
    // Run disabled periodic for all subsystems
    for (AbstractSubsystem* subsystem : subsystems) {
        subsystem->initialize();
    }
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    // Initialize the autonomous scheduler
    AutonomousControlScheduler scheduler{};

    scheduler.initialize();


    // Run disabled periodic for all subsystems
    while (true) {
        // Run the autonomous scheduler to do our routine
        scheduler.run();

        // Run periodic for all subsystems
        for (AbstractSubsystem* subsystem : subsystems) {
            subsystem->periodic();
        }

        // Delay the loop to prevent the CPU from being overwhelmed
        pros::delay(5);
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    lady_brown.set_killed(false);
    // Initialize the driver control scheduler
    DriverControlScheduler scheduler{};
    scheduler.initialize();

    while (true) {
        // Run the driver control scheduler to get inputs from controller
        scheduler.run();

        // Run periodic for all subsystems
        for (AbstractSubsystem* subsystem : subsystems) {
            subsystem->periodic();
        }

        // Delay the loop to prevent the CPU from being overwhelmed
        pros::delay(2);
    }
}
