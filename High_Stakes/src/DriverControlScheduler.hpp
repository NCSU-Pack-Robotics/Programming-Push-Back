#pragma once
#include "Command.hpp"
#include "subystems/Drivetrain.hpp"

/** A subclass of ParallelCommand. It is used during the opcontrol driving portion of the event.
 * Handles all controller input.
 */
class DriverControlScheduler : public ParallelCommand {
public:
    /** Constructor */
    DriverControlScheduler();
    /** Always returns false; This will always run during the driving portion of the event */
    bool is_complete() override;
protected:
    /** Gets instance of Controller */
    void initialize() override;

    /** Runs every tick. Checks the controller for input. Joystick input is handled directly
     * inside this method. Other input is handled through adding InstantCommands to the ParallelCommand
     */
    void periodic() override;

    /**  */
    void shutdown() override;

private:
    /** An instance of the controller */
    pros::Controller controller = pros::Controller{pros::E_CONTROLLER_MASTER};
    /** A reference to the Drivetrain */
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
};