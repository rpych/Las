#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"

namespace las::test
{
using namespace commands;
class TestDiffStagedCommand: public ICommand, public ACommandWrapper
{
public:
  TestDiffStagedCommand(CommandParams& params): ACommandWrapper(params) {}
  void runAlgorithm() override
  {
    std::cout<<"runAlgorithm for TEST  DIFF staged command"<<std::endl;
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_PUSH_STAGED);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY_1);
    auto const& filenames = (cmdLineFilenames) ? *cmdLineFilenames : std::move(getAllFilenames());
    fileParser->parse(filenames);
    fileWriter->write(fileParser->getFilesHunks());
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP_1);
  }
};

}