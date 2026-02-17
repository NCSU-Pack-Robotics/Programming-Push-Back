#pragma once

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"

class SerialHandlerSubsystem : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

protected:
    SerialHandlerSubsystem();
};
