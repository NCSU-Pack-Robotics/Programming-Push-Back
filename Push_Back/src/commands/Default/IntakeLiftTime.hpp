#pragma once

#include "IntakeTime.hpp"
#include "LiftTime.hpp"

class IntakeLiftTime : public ParallelCommand {
public:
    /**
     * Constructor for the IntakeLiftTime command.
     * @param time The time to intake for in seconds.
     * @param direction 1 for inwards, -1 for outwards.
     */
    IntakeLiftTime(double time,
        double intake_speed = Constants::Controller::MotorSpeeds::INTAKE_INWARDS,
        double lift_speed = Constants::Controller::MotorSpeeds::LIFT_UP,
        double direction = 1);

    void initialize() override;
    void shutdown() override;
};
