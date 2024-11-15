#include "FileParser.hpp"
#include "OSProxyUtils.hpp"
//#include "Utils.hpp"

namespace las::commands::common
{
using LasHunk = HunksParser::LasHunk;

FileParser::FileParser(): filesHunks{}
{}

void FileParser::parse(std::vector<std::string>& filenames)
{
  std::cout<<"FileParser::parse"<<std::endl;
  std::for_each(filenames.begin(), filenames.end(), [this](auto const& f)
  {
    std::stringstream fileStream{};
    readFileContent(fileStream, f);
    parseFileStream(fileStream, f);
  });
}

void FileParser::parseFileStream(std::stringstream& s, std::string_view filename)
{
  std::cout<<"FileParser::parseFileStream filename:"<<filename<<", len: "<<filename.length()<<std::endl;
  HunksParser hunksParser{};
  hunksParser.parseForHunks(s);
  auto hunks = std::move(hunksParser.getHunks());
  std::copy(hunks.begin(), hunks.end(), std::ostream_iterator<HunksParser::LasHunk>(std::cout));
  if (hunks.size() > 0)
  {
    filesHunks.emplace(filename, hunks);
  }
}

}
