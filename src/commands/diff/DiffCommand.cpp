#include "DiffCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffCommand::DiffCommand(): ACommandWrapper()
{}

void DiffCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF command"<<std::endl;
  std::string const filenamesBundle = getOsCommandResult(common::GitCmd::GIT_STATUS);
  auto filenames = std::move(common::getFilenamesFromStatusCmd(filenamesBundle));
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
  //next steps of las diff command
}

std::string const& DiffCommand::getOsCommandResult(common::GitCmd cmd)
{
  osCommandProxy->executeOsCommand(cmd);
  std::string const& cmdResult = osCommandProxy->getOsCommandResult();
  return cmdResult;
}

}
