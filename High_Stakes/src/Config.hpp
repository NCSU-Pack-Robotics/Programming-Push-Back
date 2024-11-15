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
          
            /** Motor speed of the intake going inwards */
            constexpr int32_t INTAKE_INWARDS = 70;
            /** Motor speed of the intake going outwards */
            constexpr int32_t INTAKE_OUTWARDS = -70;
        }
    }
}