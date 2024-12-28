#include "DiffStagedCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffStagedCommand::DiffStagedCommand(): ACommandWrapper()
{}

void DiffStagedCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF staged command"<<std::endl;
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_PUSH_STAGED);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY_1);
  std::string const filenamesBundle = osCommandProxy->executeOsCommand(common::GitCmd::GIT_DIFF_HEAD_FILES);
  auto filenames = std::move(common::getFilenamesFromStatusCmd(filenamesBundle));
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
  //next steps of las diff command
}

}
