#include "DiffStagedCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffStagedCommand::DiffStagedCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffStagedCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF staged command"<<std::endl;
  auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
  if (allFilenames.size() == 0)
  {
    return;
  }
  auto const filenamesWorkArea(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_FILES)));
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_PUSH_KEEP_INDEX);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);

  auto const& filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES)
                                             : getAllFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES);
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());

  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX_1);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP);
}

}
