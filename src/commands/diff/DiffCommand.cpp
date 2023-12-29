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
  std::string const diffBundle = getOsCommandResult(common::GitCmd::GIT_DIFF);
  fileParser->parse(diffBundle, filenamesBundle);
  //next steps of las diff command
}

std::string const& DiffCommand::getOsCommandResult(common::GitCmd cmd)
{
  osCommandProxy->executeOsCommand(cmd);
  std::string const& cmdResult = osCommandProxy->getOsCommandResult();
  return cmdResult;
}

}
