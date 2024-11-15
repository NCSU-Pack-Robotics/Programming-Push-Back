#pragma once

#include "../AbstractSubsystem.hpp"

/**
 * This subsystem controls the robot and dispatches commands during the driver-control period
 * of a match. There is another subsystem for autonomous.
 *
 * The main responsibility of this subsystem is to dispatch commands to the robot based on
 * driver input. This is done by binding commands to controller buttons.
 */
class DriverControl : public AbstractSubsystem {

};
