#pragma once
#include <memory>
#include "ICommand.hpp"
#include "ICommandExecutor.hpp"

namespace las::commands
{

class CommandExecutor: public ICommandExecutor
{
public:
  CommandExecutor();
  void execute() override;
  void setCommand(std::unique_ptr<ICommand> com);

private:
  std::unique_ptr<ICommand> command;
};

}