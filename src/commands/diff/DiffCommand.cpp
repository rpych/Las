#include "DiffCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffCommand::DiffCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffCommand::runAlgorithm()
{
  std::cout<<"Run las diff command"<<std::endl;
  auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
  if (allFilenames.size() == 0)
  {
    return;
  }
  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_DIFF_STAGED, RestoreCommand::cmdStagedAreaBackup, common::RepoState::SAVE);
  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_DIFF, RestoreCommand::cmdWorkAreaBackup, common::RepoState::SAVE);

  auto const filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_FILES)
                                            : getAllFilenames(common::GitCmd::GIT_DIFF_FILES);
  if (fileParser->parse(filenames))
  {
    fileWriter->write(fileParser->getFilesHunks());
  }

  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF, filenames, las::LasCmdOutputFile);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);

  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_APPLY_INDEX, RestoreCommand::cmdStagedAreaBackup, common::RepoState::ROLLBACK);
  osCommandProxy->executeOsCommandWithFile(common::GitCmd::GIT_APPLY, RestoreCommand::cmdWorkAreaBackup, common::RepoState::ROLLBACK);
}

}
