#include "FileParser.hpp"
#include "OSProxyUtils.hpp"

namespace las::commands::common
{

FileParser::FileParser(): filesHunks{}
{}

void FileParser::parse(std::vector<std::string> const& filenames)
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
  std::shared_ptr<LasLanguage> lasLang{getLanguage(filename)};
  HunksParser hunksParser{lasLang};
  hunksParser.parseForHunks(s);
  auto hunks = std::move(hunksParser.getHunks());
  std::copy(hunks.begin(), hunks.end(), std::ostream_iterator<LasHunk>(std::cout));
  if (hunks.size() > 0)
  {
    std::cout<<"RPY::getHunks>0::"<<std::endl;
    filesHunks.emplace(filename, hunks);
  }
}

}
