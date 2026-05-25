TimelineCommand::TimelineCommand(std::unique_ptr<ProgressCommand> mainCommand, std::vector<Checkpoint>&& checkpoints)
: ParallelCommand({}), mainCommand(std::move(mainCommand)), checkpoints(std::move(checkpoints)) {}

void TimelineCommand::periodic() {
    // This loop checks mainCommand progress to activate checkpoints
    if (this->mainCommand) {
        // progress called here to avoid calling a null pointer on final loop
        const double progress = mainCommand->get_progress();

        if (this->mainCommand->is_complete()) {
            mainCommand->shutdown();
            //printf("Main shutdown\n");
            mainCommand = nullptr;
        } else {
            mainCommand->run();
        }

        //printf("Checkpoint progress: %f\n", progress);

        // Once a checkpoint is reached, remove it so it doesn't keep adding the same command on subsequent loops
        if (checkpoints.size() == 1) {
            if (checkpoints[0].activationPoint <= progress) {
                //printf("Checkpoint \n");
                ParallelCommand::add_command(std::move(checkpoints[0].command));
                checkpoints.clear();
            }
        } else {
            for (auto it = checkpoints.begin(); it != checkpoints.end();) {
                if (it->activationPoint <= progress) {
                    // ParallelCommand:: is added for clarity
                    ParallelCommand::add_command(std::move(it->command));
                    printf("Checkpoint \n");
                    it = checkpoints.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    // Runs all commands that were part of checkpoints. mainCommand does NOT get run here
    ParallelCommand::periodic();
}

bool TimelineCommand::is_complete() {
    if (this->mainCommand) {
        if (mainCommand->is_complete() && ParallelCommand::is_complete()) {
            //printf("is_complete\n");
            return true;
        }
    }
    return false;
}

void TimelineCommand::shutdown() {}

void TimelineCommand::initialize() {}