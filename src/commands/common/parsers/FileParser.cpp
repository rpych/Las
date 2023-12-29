#include "FileParser.hpp"
#include "OSProxyUtils.hpp"
#include "Utils.hpp"

namespace las::commands::common
{

FileParser::FileParser(): filenames{}, filesHunks{}, diffParser{std::make_unique<DiffParser>()}
{}

void FileParser::parse(std::string_view diffBundle, std::string_view filenamesBundle)
{
  std::cout<<"FileParser::parse"<<std::endl;
  setFilenames(filenamesBundle);
  diffParser->parse(diffBundle);
  std::for_each(filenames.begin(), filenames.end(), [this](auto const& f)
  {
    std::stringstream fileStream{};
    readFileContent(fileStream, f);
    parseFileStream(fileStream, f);
  });
}

void FileParser::parseFileStream(std::stringstream& s, std::string_view filename)
{
  std::cout<<"FileParser::parseFileStream"<<std::endl;
  //^^las begin^^ Rafal
  HunksParser hunksParser{};
  hunksParser.parseForHunks(s);
  //^^las end^^                  Pych
  auto hunks = std::move(hunksParser.getHunks());
  std::cout<<"RPY"<<std::endl;
  std::copy(hunks.begin(), hunks.end(), std::ostream_iterator<HunksParser::LasHunk>(std::cout));
  filesHunks.emplace(filename, hunks);
}

void FileParser::setFilenames(std::string_view filenamesBundle)
{
  auto filenamesExtacted = getFilenamesFromStatusCmd(filenamesBundle);
  filenames = std::move(filenamesExtacted);
}

}