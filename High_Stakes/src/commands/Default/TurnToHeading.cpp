#include "TurnToHeading.hpp"

#include "../../math/Utils.hpp"

TurnToHeading::TurnToHeading(double target_heading, const double tolerance) :
    tolerance(tolerance), controller(0, 0, 0) {

    this->target_heading = utils.ensure_positive_radians(target_heading);
}

/**
 * Helper method to get the difference between two angles. This method will always return the
 * shortest distance between the two angles. The method will return a negative values if angle
 * <code>b</code> is clockwise from angle <code>a</code> and a positive value if angle
 * <code>a</code> is clockwise from angle <code>b</code>. In other words, the method will return a
 * positive value if the robot has to turn left to get from <code>a</code> to <code>b</code> and a
 * negative value if the robot has to turn right to get from <code>a</code> to <code>b</code>.
 */
double angle_difference(double a, double b) {
    a = utils.ensure_positive_radians(a);
    b = utils.ensure_positive_radians(b);

    double diff = b - a;

    // Need to adjust the error if it is greater than PI (there is a shorter way to turn)
    if (fabs(diff) > M_PI) {
        if (diff < 0) {
            diff += Constants::Math::TAU;
        } else {
            diff -= Constants::Math::TAU;
        }

        diff = fmod(diff, Constants::Math::TAU);
    }

    return diff;
}

void TurnToHeading::initialize() {
    // Get the error in the heading
    const double current_heading = this->drivetrain.get_pose().heading;
    double error = angle_difference(current_heading, target_heading);

    // Ensue this is positive for the following scaling to work correctly
    error = utils.ensure_positive_radians(error);

    // Scale Kp between PI/4:8000 and PI:6000
    double m = (6000 - 8000) / (M_PI - M_PI/4);
    double b = 8000 - m * M_PI/4;
    double Kp = m * error + b;
    Kp = std::clamp(Kp, 6000.0, 8000.0);

    // Scale Ki between PI/4:0 and PI:1200
    m = (1200 - 0) / (M_PI - M_PI/4);
    b = 1200 - m * M_PI;
    double Ki = m * error + b;
    Ki = std::clamp(Ki, 0.0, 1200.0);

    // Scale Kd between PI/4:0 and PI:120
    m = (120 - 0) / (M_PI - M_PI/4);
    b = 120 - m * M_PI;
    double Kd = m * error + b;
    Kd = std::clamp(Kd, 0.0, 120.0);

    // Construct the controller with the scaled values
    this->controller = PID(Kp, Ki, Kd);
}

void TurnToHeading::periodic() {
    // Get the error in the heading
    const double current_heading = this->drivetrain.get_pose().heading;
    const double error = angle_difference(current_heading, target_heading);

    // Calculate the PID output
    const double voltage = controller.calculate(error);

    /* Left voltage is negative, b/c when error is positive, the robot needs to default to turning
     * to the left. When error is negative, the robot should be turning right. */

    // Set voltages
    const double left_voltage = -voltage;
    const double right_voltage = voltage;
    drivetrain.set_voltage(left_voltage, right_voltage);
}

bool TurnToHeading::is_complete() {
    double current_heading = this->drivetrain.get_pose().heading;
    current_heading = utils.ensure_positive_radians(current_heading);

    if (fabs(target_heading - current_heading) < tolerance) {
        return true;
    }

    return false;
}

void TurnToHeading::shutdown() {}
