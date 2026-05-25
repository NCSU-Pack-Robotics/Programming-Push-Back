/**
 * A command that follows the progression of a <code>ProgressCommand</code> and activates additional
 * commands at certain Checkpoints that are done in parallel to the main command.
 */
class TimelineCommand : public ParallelCommand {
public:
    /**
    * A Checkpoint contains both an activation point along the main command
    * and a command to be executed at that point.
    */
    struct Checkpoint {
        /**
        * The actual constructor for Checkpoint. Takes in an activation point corresponding to
        * an amount of progress along the main command, and a command that starts at that point.
        * @param activationPoint The amount of progress along the main command at which another command activates
        * @param command The command to be executed at the activation point
        */
        Checkpoint(double activationPoint, std::unique_ptr<Command> command) {
            this->activationPoint = activationPoint;
            this->command = std::move(command);
        }

        double activationPoint;
        std::unique_ptr<Command> command;
    };

    /**
    * A constructor that uses a main ProgressCommand and an initializer list of checkpoints to activate
    * additional commands.
    * @param mainCommand The command whose progress is tracked to activate other commands
    * @param checkpoints The list of Checkpoints that will activate throughout mainCommand
    */
    TimelineCommand(std::unique_ptr<ProgressCommand> mainCommand, std::vector<Checkpoint>&& checkpoints);

    std::unique_ptr<ProgressCommand> mainCommand;
    std::vector<Checkpoint> checkpoints;

    /**
    * Runs mainCommand and any other commands from Checkpoints, and checks mainCommand progress
    * to activate more Checkpoints
    */
    void periodic() override;

    bool is_complete() override;

    // These shouldn't need to be overridden, but it throws an error if they aren't and TimelineCommand is used
    void shutdown() override;
    void initialize() override;
};