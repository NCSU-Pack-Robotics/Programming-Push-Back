#pragma once

#include "Config.hpp"
#include "comms/AbstractComm.hpp"

#include "main.h"
#include "ports.hpp"
#include "serial.hpp"

class BrainComm : public AbstractComm {
    pros::Serial tx{Ports::AUX_TX_PORT, Constants::AUX_COMM::BAUD_RATE};
    pros::Serial rx{Ports::AUX_RX_PORT, Constants::AUX_COMM::BAUD_RATE};

    pros::Mutex mutex{};

public:
    BrainComm();
    ~BrainComm() override = default;

    size_t read(unsigned char* buf, size_t count) override;
    void write(unsigned char* buf, size_t count) override;

    void mutex_lock() override;
    void mutex_unlock() override;
};
