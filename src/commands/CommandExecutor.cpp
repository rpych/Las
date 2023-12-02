#include "CommandExecutor.hpp"

namespace las::commands
{

CommandExecutor::CommandExecutor()
{}

void CommandExecutor::execute()
{
  command->runAlgorithm();
}

void CommandExecutor::setCommand(std::unique_ptr<ICommand> com)
{
  this->command = std::move(com);
}

}
