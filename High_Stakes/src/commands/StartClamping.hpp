#include "Command.hpp"
class StartClamping : public InstantCommand {
public:
    StartClamping();
protected:
    void execute() override;
};