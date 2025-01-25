#pragma once
#include <memory>
#include "ICommand.hpp"
#include "../ui/CmdParser.hpp"
#include "../commands/cut/CutCommand.hpp"
#include "../commands/diff/DiffCommand.hpp"
#include "../commands/diff/DiffHeadCommand.hpp"
#include "../commands/diff/DiffStagedCommand.hpp"

namespace las::commands
{
class ICommandExecutor
{
public:
  virtual void execute() = 0;
  virtual void setCommand(std::unique_ptr<las::ui::ICmdParser> parser) = 0;
  virtual ~ICommandExecutor() = default;
};

class CommandExecutor: public ICommandExecutor
{
public:
  CommandExecutor();
  void execute() override;
  void setCommand(std::unique_ptr<las::ui::ICmdParser> parser) override;

protected:
  virtual std::unique_ptr<ICommand> createCommand(std::unique_ptr<las::ui::ICmdParser> parser);
  common::LasCmdOpts getExecutionMode(std::set<common::LasCmdOpts> const& options);
  std::optional<FilenamesVector> getFilenames(FilenamesVector const& filenames);
  std::unique_ptr<ICommand> command;
  //friend class TestCommandExecutor;
};

}
