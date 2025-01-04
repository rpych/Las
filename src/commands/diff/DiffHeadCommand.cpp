#include "DiffHeadCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffHeadCommand::DiffHeadCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffHeadCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF HEAD command"<<std::endl;
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);
  auto const& filenames = (cmdLineFilenames) ? *cmdLineFilenames : std::move(getAllFilenames());
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
  //next steps of las diff command
}

}
