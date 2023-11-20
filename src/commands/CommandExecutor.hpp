#pragma once
#include <memory>
#include "ICommand.hpp"
#include "common/OSCommandProxy.hpp"

// namespace las::commands::common
// {
//   class AOSCommandProxy<GitCmd>;
//   enum class GitCmd;
// }

namespace las::commands
{
class ICommandExecutor
{
public:
  virtual void execute() = 0;
  virtual void setCommand(std::unique_ptr<ICommand> com) = 0;
  virtual ~ICommandExecutor() = default;
};

class CommandExecutor: public ICommandExecutor
{
public:
  CommandExecutor();
  void execute() override;
  void setCommand(std::unique_ptr<ICommand> com) override;

private:
  std::unique_ptr<ICommand> command;
  std::unique_ptr<common::AOSCommandProxy<common::GitCmd>> osCommandProxy;
};

}
