#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"
#include "testcases/Utils.hpp"
#include "../src/commands/restore/RestoreCommand.hpp"

namespace las::test
{
using namespace commands;
class TestDiffStagedCommand: public ICommand, public ACommandWrapper
{
public:
  TestDiffStagedCommand(CommandParams& params): ACommandWrapper(params) {}
  std::string GIT_APPLY_WORK_AREA{std::format("git apply {} --include=", RestoreCommand::cmdWorkAreaBackup)};
  void runAlgorithm() override
  {
    std::cout<<"runAlgorithm for TEST  DIFF staged command"<<std::endl;
    auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
    if (allFilenames.size() == 0)
    {
      return;
    }
    auto const filenamesWorkArea(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_FILES)));
    osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_DIFF_STAGED, RestoreCommand::cmdStagedAreaBackup, common::RepoState::SAVE);
    osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_DIFF, RestoreCommand::cmdWorkAreaBackup, common::RepoState::SAVE);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
    osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_APPLY_INDEX, RestoreCommand::cmdStagedAreaBackup, common::RepoState::ROLLBACK);

    auto const filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES)
                                              : getAllFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES);
    if (fileParser->parse(filenames))
    {
      fileWriter->write(fileParser->getFilesHunks());
    }

    for (auto const& f: filenamesWorkArea)
    {
      std::string cmdForFile{std::format("{}{}", GIT_APPLY_WORK_AREA, f)};
      if (std::find(filenames.begin(), filenames.end(), f) == filenames.end())
      {
        executeOsCommandWithoutResult(cmdForFile);
      }
    }
  }
};

}