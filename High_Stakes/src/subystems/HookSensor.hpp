#pragma once
#include <memory>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
class HookSensor : public AbstractSubsystem {
    friend AbstractSubsystem;
public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /** Returns true if something is blocking the sensor */
    bool isBlocked();


private:
    /** A unique pointer to the hook sensor */
    std::unique_ptr<pros::adi::AnalogIn> hook_sensor;

protected:
    HookSensor();
};