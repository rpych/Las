#include "DiffCommand.hpp"
#include <iostream>

namespace las::commands
{

DiffCommand::DiffCommand(CommandParams& params): ACommandWrapper(params)
{}

void DiffCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for DIFF command"<<std::endl;
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_PUSH_STAGED);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);
  //!!!!!!!!!!!!!!!GIT_DIFF_FILES in the end!!!!!!!!!!!!!!!!!!!!!!!!
  auto const& filenames = (cmdLineFilenames) ? *cmdLineFilenames : std::move(getAllFilenames());
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_DIFF);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_RESET_HARD);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP);
  // osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_POP_INDEX);
  //next steps of las diff command
}

}
