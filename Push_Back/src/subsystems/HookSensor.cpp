#include "HookSensor.hpp"

#include "../ports.hpp"

HookSensor::HookSensor() {

}

void HookSensor::initialize() {
     hook_sensor = std::make_unique<pros::adi::AnalogIn>(Ports::HOOK_SENSOR_BRAIN_PORT);
     hook_sensor->calibrate();
}

void HookSensor::periodic() {
}

void HookSensor::disabled_periodic() {

}

void HookSensor::shutdown() {

}

bool HookSensor::isBlocked() const {
     return (hook_sensor->get_value_calibrated() < 2000);
}
