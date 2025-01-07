#include "CutCommand.hpp"


namespace las::commands
{

CutCommand::CutCommand(CommandParams& params) : ACommandWrapper(params)
{}

void CutCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for CUT command"<<std::endl;
  auto const& filenames = (cmdLineFilenames) ? *cmdLineFilenames : std::move(getAllFilenames());
  fileParser->parse(filenames);
  fileWriter->write(fileParser->getFilesHunks());
}

}
