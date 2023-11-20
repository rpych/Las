#include "CommandExecutor.hpp"

namespace las::commands
{

CommandExecutor::CommandExecutor(): osCommandProxy(std::make_unique<common::OSCommandProxy<common::GitCmd>>())
{}

void CommandExecutor::execute()
{
  osCommandProxy->executeOsCommand(common::GitCmd::GIT_DIFF);
  std::string& cmdResult = osCommandProxy->getOsCommandResult();
  command->runAlgorithm();
}

void CommandExecutor::setCommand(std::unique_ptr<ICommand> com)
{
  this->command = std::move(com);
}


}
