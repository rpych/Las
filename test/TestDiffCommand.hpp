#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"
#include "../src/commands/restore/RestoreCommand.hpp"

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
    auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
    if (allFilenames.size() == 0)
    {
      return;
    }
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);

    auto const& filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_FILES)
                                               : getAllFilenames(common::GitCmd::GIT_DIFF_FILES);
    fileParser->parse(filenames);
    fileWriter->write(fileParser->getFilesHunks());

    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP);
  }
};

}