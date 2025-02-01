#include "HookSensor.hpp"

#include "../ports.hpp"

HookSensor::HookSensor() {

}

void HookSensor::initialize() {
     hook_sensor = std::make_unique<pros::adi::DigitalIn>(Ports::HOOK_SENSOR_BRAIN_PORT);
}

void HookSensor::periodic() {

}

void HookSensor::disabled_periodic() {

}

void HookSensor::shutdown() {

}

bool HookSensor::isBlocked() {
     return !hook_sensor->get_value();
}
