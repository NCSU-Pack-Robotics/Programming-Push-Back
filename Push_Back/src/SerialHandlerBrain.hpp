#pragma once

#include "SerialHandler.hpp"
#include "../include/pros/rtos.h"

class SerialHandlerBrain : public SerialHandler {
    pros::Mutex mutex;
public:
    template <typename T>
    Buffer get_buffers() {
        mutex.lock();
        Buffer buffer = this->buffers[T::id];
        mutex.unlock();
        return buffer;
    }

    template <typename T>
    std::optional<Packet> pop_latest()
    {
        return this->get_buffers<T>().pop_latest();
    }
};