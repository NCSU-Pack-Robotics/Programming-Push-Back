#pragma once
#include <memory>
#include "../AbstractSubsystem.hpp"
#include "../../include/main.h"

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
    std::unique_ptr<pros::adi::DigitalOut> arm_piston;

    bool enabled;

protected:
    Arm();
};