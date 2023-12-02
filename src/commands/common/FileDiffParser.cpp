#include "FileDiffParser.hpp"
#include "Utils.hpp"

namespace las::commands::common
{

FileDiffParser::FileDiffParser(): filenames{}
{}

void FileDiffParser::parse(std::string_view filenamesBundle)
{
  setFilenames(filenamesBundle);
  std::cout<<"FileDiffParser::parse"<<std::endl;
}

void FileDiffParser::setFilenames(std::string_view filenamesBundle)
{
  auto filenamesExtacted = getFilenamesFromStatusCmd(filenamesBundle);
  filenames = std::move(filenamesExtacted);
}

}