#include "Command.hpp"
class LadyBrownDown : public InstantCommand {
public:
    LadyBrownDown();
protected:
    void execute() override;
};