#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"

namespace las::test
{
using namespace commands;
class TestDiffHeadCommand: public ICommand, public ACommandWrapper
{
public:
  TestDiffHeadCommand(CommandParams& params): ACommandWrapper(params) {}
  void runAlgorithm() override
  {
    std::cout<<"runAlgorithm for TEST DIFF HEAD command"<<std::endl;
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);
    auto const& filenames = (cmdLineFilenames) ? *cmdLineFilenames : std::move(getAllFilenames());
    fileParser->parse(filenames);
    fileWriter->write(fileParser->getFilesHunks());
    // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
    // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
    // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
    //next steps of las diff command
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP);
  }
};

}