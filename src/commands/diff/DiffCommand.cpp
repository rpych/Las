#include "DiffCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffCommand::DiffCommand(): ACommandWrapper()
{}

void DiffCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF command"<<std::endl;
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);
  std::string const filenamesBundle = getOsCommandResult(common::GitCmd::GIT_STATUS);
  auto filenames = std::move(common::getFilenamesFromStatusCmd(filenamesBundle));
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF_HEAD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP);
  //next steps of las diff command
}

std::string const& DiffCommand::getOsCommandResult(common::GitCmd cmd)
{
  osCommandProxy->executeOsCommand(cmd);
  std::string const& cmdResult = osCommandProxy->getOsCommandResult();
  return cmdResult;
}

}
