#include "ProgressCommand.hpp"

std::string ProgressCommand::to_string() const
{
    return std::format("{}(Progress: {}):\n", get_name(), get_progress());
}

std::string ProgressCommand::get_name() const
{
    return "ProgressCommand";
}
