#include "DiffStagedCommand.hpp"
#include "../restore/RestoreCommand.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace las::commands
{

DiffStagedCommand::DiffStagedCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffStagedCommand::runAlgorithm()
{
  std::cout<<"Run las diff staged command"<<std::endl;
  auto const indexAreaFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES)));
  if (indexAreaFilenames.size() == 0)
  {
    return;
  }
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

  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD, filenames);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);

  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_APPLY_INDEX, RestoreCommand::cmdStagedAreaBackup, common::RepoState::ROLLBACK);
  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_APPLY, RestoreCommand::cmdWorkAreaBackup, common::RepoState::ROLLBACK);
}

}
