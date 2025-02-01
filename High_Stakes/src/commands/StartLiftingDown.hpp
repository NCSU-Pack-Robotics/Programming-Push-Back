#include "Command.hpp"
class StartLiftingDown : public InstantCommand {
public:
    StartLiftingDown();
protected:
    void execute() override;
};