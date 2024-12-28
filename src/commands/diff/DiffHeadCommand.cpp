#include "DiffHeadCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffHeadCommand::DiffHeadCommand(): ACommandWrapper()
{}

void DiffHeadCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF HEAD command"<<std::endl;
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);
  std::string const filenamesBundle = osCommandProxy->executeOsCommand(common::GitCmd::GIT_DIFF_HEAD_FILES);
  auto filenames = std::move(common::getFilenamesFromStatusCmd(filenamesBundle));
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
  //next steps of las diff command
}

}
