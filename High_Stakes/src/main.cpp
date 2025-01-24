#include "../include/main.h"
#include "subystems/Drivetrain.hpp"
#include "subystems/Lift.hpp"
#include "Config.hpp"
#include "subystems/Intake.hpp"
#include "DriverControlScheduler.hpp"
#include "asset.hpp"
#include "commands/PurePursuit.hpp"

// Create all subsystems:
Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
Lift& lift = AbstractSubsystem::get_instance<Lift>();
Intake& intake = AbstractSubsystem::get_instance<Intake>();

// Add subsystems to vector for iteration
std::vector<AbstractSubsystem*> subsystems = {&drivetrain, &lift, &intake};

/** Controller object */
pros::Controller controller{pros::E_CONTROLLER_MASTER};

DriverControlScheduler driver_control_scheduler{};

// example_txt is of type asset&
ASSET(examplePath_txt);

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
    const auto follow_path = new PurePursuit(examplePath_txt);

    // Run disabled periodic for all subsystems
    while (true) {
        follow_path->run();

        for (AbstractSubsystem* subsystem : subsystems) {
            subsystem->periodic();
        }
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
    while (true) {
        // Run the driver control scheduler, which gets input and sets all the motors.
        driver_control_scheduler.run();
        // Run periodic for all subsystems
        for (AbstractSubsystem* subsystem : subsystems) {
            subsystem->periodic();
        }

        // Delay the loop to prevent the CPU from being overwhelmed
        pros::delay(20);
    }
}
