#include "../AbstractSubsystem.hpp"
class LadyBrown : AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

protected:
    LadyBrown();
};