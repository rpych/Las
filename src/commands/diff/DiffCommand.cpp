#include "DiffCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffCommand::DiffCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF command"<<std::endl;
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

  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
}

}
