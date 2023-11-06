#pragma once
#include <memory>
#include "ICommand.hpp"

namespace las::commands
{

class ICommandExecutor
{
public:
  virtual void execute() = 0;
  virtual ~ICommandExecutor() = default;
};

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