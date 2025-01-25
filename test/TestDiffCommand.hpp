#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"

namespace las::test
{
using namespace commands;
class TestDiffCommand: public ICommand, public ACommandWrapper
{
public:
  TestDiffCommand(CommandParams& params): ACommandWrapper(params) {}
  void runAlgorithm() override
  {
    std::cout<<"runAlgorithm for TEST DIFF command"<<std::endl;
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_PUSH_STAGED);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);
    auto const& filenames = (cmdLineFilenames) ? *cmdLineFilenames : std::move(getAllFilenames());
    fileParser->parse(filenames);
    fileWriter->write(fileParser->getFilesHunks());
    // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF);
    // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP_1);
  }
};

}