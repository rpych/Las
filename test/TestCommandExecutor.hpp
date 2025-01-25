#pragma once
#include "../src/commands/CommandExecutor.hpp"
#include "TestDiffCommand.hpp"
#include "TestDiffHeadCommand.hpp"
#include "TestDiffStagedCommand.hpp"

namespace las::test
{
using namespace commands;
class TestCommandExecutor : public CommandExecutor
{
public:
  TestCommandExecutor() = default;
private:
  std::unique_ptr<ICommand> createCommand(std::unique_ptr<las::ui::ICmdParser> parser) override
  {
    std::cout<<"TestCommandExecutor::createCommand"<<std::endl;
    auto mode{getExecutionMode(parser->getOptions())};
    auto filenames {std::move(getFilenames(parser->getFilenames()))};
    std::unique_ptr<common::AFileParser> fileParser{std::make_unique<common::FileParser>()};
    auto params {CommandParams{.fileParser=std::move(fileParser),
                              .mode=mode,
                              .filenames=filenames}};
    switch (parser->getCommand())
    {
      case common::LasCmd::CUT:
        return std::make_unique<las::commands::CutCommand>(params);
        break;
      case common::LasCmd::DIFF:
        return std::make_unique<TestDiffCommand>(params);
        break;
      case common::LasCmd::DIFF_HEAD:
        return std::make_unique<TestDiffHeadCommand>(params);
        break;
      case common::LasCmd::DIFF_STAGED:
        return std::make_unique<TestDiffStagedCommand>(params);
        break;
      default:
        return nullptr;
    }
  }
};


}