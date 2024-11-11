#pragma once
#include <cstdint>


namespace Constants {
    namespace Controller {
        /** Constants for the speed used when motors are activated using controller */
        namespace MotorSpeeds {
            /** Lift speed for the lift moving upwards */
            constexpr int32_t LIFT_UP = 30;
            /** Lift speed for the lift moving backwards */
            constexpr int32_t LIFT_DOWN = -30;
        }
    }
}