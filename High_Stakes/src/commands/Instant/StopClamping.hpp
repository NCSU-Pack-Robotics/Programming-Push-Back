#include "../../Command.hpp"
class StopClamping : public InstantCommand {
public:
    StopClamping();
protected:
    void execute() override;
};