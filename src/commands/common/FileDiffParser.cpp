#include "FileDiffParser.hpp"
#include "OSProxyUtils.hpp"
#include "Utils.hpp"
#include <algorithm>

namespace las::commands::common
{

FileDiffParser::FileDiffParser(): filenames{}, filesHunks{}
{}

void FileDiffParser::parse(std::string_view filenamesBundle)
{
  std::cout<<"FileDiffParser::parse"<<std::endl;
  setFilenames(filenamesBundle);
  std::for_each(filenames.begin(), filenames.end(), [this](auto const& f)
  {
    std::stringstream fileStream{};
    readFileContent(fileStream, f);
    parseFileStream(fileStream);
  });
}

void FileDiffParser::parseFileStream(std::stringstream& s)
{
  std::cout<<"FileDiffParser::parseFileStream"<<std::endl;
}

void FileDiffParser::setFilenames(std::string_view filenamesBundle)
{
  auto filenamesExtacted = getFilenamesFromStatusCmd(filenamesBundle);
  filenames = std::move(filenamesExtacted);
}

}