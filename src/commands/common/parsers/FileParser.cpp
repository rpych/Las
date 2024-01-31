#include "FileParser.hpp"
#include "OSProxyUtils.hpp"
#include "Utils.hpp"

namespace las::commands::common
{
using LasHunk = HunksParser::LasHunk;

FileParser::FileParser(): filenames{}, filesHunks{}, diffParser{std::make_unique<DiffParser>(filenames)}
{}

std::vector<std::string> const& FileParser::getFilenames()
{
  return filenames;
}

std::vector<LasHunk> const& FileParser::getLasHunksForFile(std::string const& filename)
{
  return filesHunks.at(filename);
}

std::vector<DiffHunk> const& FileParser::getDiffHunksForFile(std::string const& filename)
{
  return diffParser->getDiffHunks(filename);
}

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
  std::cout<<"FileParser::parseFileStream filename:"<<filename<<", len: "<<filename.length()<<std::endl;
  //^^las begin^^
  HunksParser hunksParser{};
  hunksParser.parseForHunks(s, diffParser->getDiffHunks(std::string{filename}));
  //^^las end^^
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