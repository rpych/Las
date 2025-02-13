#include "CutCommand.hpp"


namespace las::commands
{

CutCommand::CutCommand(CommandParams& params) : ACommandWrapper(params)
{}

void CutCommand::runAlgorithm()
{
  std::cout<<"Run las cut command"<<std::endl;
  auto const filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)
                                            : getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES);
  if (fileParser->parse(filenames))
  {
    fileWriter->write(fileParser->getFilesHunks());
  }
}

}
