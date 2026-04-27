#pragma once

#include <adi.hpp>
#include <ports.hpp>
#include <architecture/AbstractSubsystem.hpp>

class Lift : public AbstractSubsystem
{
    friend class AbstractSystem;

    bool enabled;
    pros::adi::DigitalOut lift_piston = pros::adi::DigitalOut(Ports::LIFT_PISTON_PORT);

public:
    void periodic() override;

    void initialize() override;

    void toggle();

    void disabled_periodic() override;

    void shutdown() override;
};
