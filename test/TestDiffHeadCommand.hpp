#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"
#include "../src/commands/restore/RestoreCommand.hpp"

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
    auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
    if (allFilenames.size() == 0)
    {
      return;
    }
  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_DIFF_STAGED, RestoreCommand::cmdStagedAreaBackup, common::RepoState::SAVE);
  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_DIFF, RestoreCommand::cmdWorkAreaBackup, common::RepoState::SAVE);

    auto const filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)
                                              : allFilenames;
    if (fileParser->parse(filenames))
    {
      fileWriter->write(fileParser->getFilesHunks());
    }
  }
};

}