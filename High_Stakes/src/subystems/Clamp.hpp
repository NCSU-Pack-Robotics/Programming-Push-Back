#pragma once
#include <memory>
#include "../AbstractSubsystem.hpp"
#include "../../include/main.h"

class Clamp : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /** Sets if the clamp is enabled or not */
    void set_enabled(bool enabled) const;

private:
    std::unique_ptr<pros::adi::DigitalOut> clamp_piston;

protected:
    Clamp();
};