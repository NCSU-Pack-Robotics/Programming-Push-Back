#pragma once

#include "TurnToHeading.hpp"

class TurnDegrees : public TurnToHeading {
    double to_turn;

public:
    TurnDegrees(double degrees, double tolerance, double max_time=std::numeric_limits<double>::max());

    void initialize() override;
};
