#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../ports.hpp"

class Clamp : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /** Sets if the clamp is enabled or not */
    void set_enabled(bool enabled);

    /** Toggles the state of the clamp */
    void toggle();

private:
    pros::adi::DigitalOut clamp_piston = pros::adi::DigitalOut(Ports::CLAMP_BRAIN_PORT);

    bool enabled;

protected:
    Clamp();
};