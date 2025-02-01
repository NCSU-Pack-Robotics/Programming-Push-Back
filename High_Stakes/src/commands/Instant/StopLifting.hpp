#include "../../Command.hpp"
class StopLifting : public InstantCommand {
public:
    StopLifting();
protected:
    void execute() override;
};