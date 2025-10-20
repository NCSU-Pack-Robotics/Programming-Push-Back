#include "AbstractMotorSubsystem.hpp"

void AbstractMotorSubsystem::move() {
    if (braking) {
        brake_now();
        return;
    } else {
        for (auto& motor : motors) {
            if (motor) {
                motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
            }
        }
    }

    switch (drive_type) {
        case Constants::DriveType::POWER: {
            for (auto& motor : motors) {
                if (motor) {
                    motor->move(drive_power);
                }
            }
            break;
        }
        case Constants::DriveType::VOLTAGE: {
            for (auto& motor : motors) {
                if (motor) {
                    motor->move_voltage(drive_voltage);
                }
            }
            break;
        }
    }
}


void AbstractMotorSubsystem::disabled_move() {

}

void AbstractMotorSubsystem::shutdown() {
    brake_now();
}

void AbstractMotorSubsystem::set_voltage(int32_t voltage) {
    drive_voltage = std::clamp(voltage, INT32_C(-12000), INT32_C(12000));
    
    drive_type = Constants::DriveType::VOLTAGE;
}

void AbstractMotorSubsystem::set_power(int32_t power) {
    drive_power = std::clamp(power, INT32_C(-127), INT32_C(127));
   
    drive_type = Constants::DriveType::POWER;
}

bool AbstractMotorSubsystem::set_braking(bool braking) {
    // Don't need to do anything if its already in the right mode
    if (braking == this->braking) return braking;
    bool old = this->braking;

    this->braking = braking;
    return old;
}

void AbstractMotorSubsystem::brake_now() {
    for (auto& motor : motors) {
        if (motor) {
            motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
        }
    }

    drive_power = 0;

    drive_voltage = 0;

    for (auto& motor : motors) {
        if (motor) {
            motor->brake();
        }
    }
}