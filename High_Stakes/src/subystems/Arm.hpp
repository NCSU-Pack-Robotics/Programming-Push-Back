#pragma once
#include <memory>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../ports.hpp"


class Arm : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /** Sets if the arm is enabled or not */
    void set_enabled(bool enabled);

    /** Toggles the state of the arm */
    void toggle();

private:
    pros::adi::DigitalOut arm_piston = pros::adi::DigitalOut(Ports::ARM_BRAIN_PORT);

    bool enabled;

protected:
    Arm();
};