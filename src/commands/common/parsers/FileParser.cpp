#include "FileParser.hpp"
#include "../OSProxyUtils.hpp"

namespace las::commands::common
{

FileParser::FileParser(): filesHunks{}
{}

bool FileParser::parse(std::vector<std::string> const& filenames)
{
  logLasDebug("FileParser::parse");
  bool result{true};
  std::for_each(filenames.begin(), filenames.end(), [this, &result](auto const& f)
  {
    std::stringstream fileStream{};
    readFileContent(fileStream, f);
    if (not parseFileStream(fileStream, f)) { result = false; }
  });
  return result;
}

bool FileParser::parseFileStream(std::stringstream& s, std::string_view filename)
{
  logLasDebug("FileParser::parseFileStream filename: {}", filename);
  std::shared_ptr<LasLanguage> lasLang{getLanguage(filename)};
  HunksParser hunksParser{lasLang};
  if (not hunksParser.parseForHunks(s))
  {
    logLasError("Above problem in file: {}", filename);
    return false;
  }
  auto hunks = std::move(hunksParser.getHunks());
  if (hunks.size() > 0)
  {
    filesHunks.emplace(filename, hunks);
  }
  return true;
}

}
