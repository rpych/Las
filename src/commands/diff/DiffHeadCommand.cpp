#include "DiffHeadCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffHeadCommand::DiffHeadCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffHeadCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF HEAD command"<<std::endl;
  auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
  if (allFilenames.size() == 0)
  {
    return;
  }
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);

  auto const& filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)
                                            : allFilenames;
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());

  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
}

}
