#include "DiffParser.hpp"
#include "../../../Const.hpp"
#include <iostream>

namespace las::commands::common
{

void DiffParser::parse(std::string_view content)
{
  std::stringstream s{};
  s << content;
  while(s)
  {
    /*diff tag format: e.g @@ -16,11 +18,12 @@ */
    std::string line{};
    std::getline(s, line);
    if (line.empty())
    {
      continue;
    }
    else if (shouldNewFileBeProcessed(line))
    {
      auto filename {readFilename(line)};
      setCurrentFilename(std::make_optional(filename));
    }
    else if (isFileRemoved(line))
    {
      setCurrentFilename(std::nullopt);
    }

    createDiffHunkIfPossible(line);
  }
}

void DiffParser::setCurrentFilename(std::optional<std::string> filename)
{
  currentFilename = filename;
}

void DiffParser::createDiffHunkIfPossible(std::string_view line)
{
  if (currentFilename.has_value() and isDiffTag(line))
    {
      std::cout<<"RPY::DiffParser::createDiffHunkIfPossible isDiffTag "<<line<<std::endl;
      auto minusIt = std::find(line.begin(), line.end(), '-');
      auto firstCommaIt = std::find(line.begin(), line.end(), ',');
      auto originEnd = std::find(firstCommaIt, line.end(), ' ');
      auto plusIt = std::find(firstCommaIt+1, line.end(), '+');
      auto secCommaIt = std::find(firstCommaIt+1, line.end(), ',');
      auto newEnd = std::find(secCommaIt, line.end(), ' ');

      auto startLineOrig = static_cast<uint64_t>(std::stoi(std::string(minusIt+1, firstCommaIt)));
      auto numOfLinesOrig = static_cast<uint32_t>(std::stoi(std::string(firstCommaIt+1, originEnd)));

      auto startLineNew = static_cast<uint64_t>(std::stoi(std::string(plusIt+1, secCommaIt)));
      auto numOfLinesNew = static_cast<uint32_t>(std::stoi(std::string(secCommaIt+1, newEnd)));

      std::cout<<"startLineOrig: "<<startLineOrig<<", numOfLinesOrig: "<<numOfLinesOrig<<",startLineNew: "<<startLineNew<<", numOfLinesNew: "<<numOfLinesNew<<std::endl;

      DiffHunk hunk = {startLineOrig, numOfLinesOrig, startLineNew, numOfLinesNew, std::string{""} };

      auto f{currentFilename.value()};
      auto fileHunksIt = diffHunks.find(f);
      if (fileHunksIt == diffHunks.end())
      {
        diffHunks.emplace(f, std::vector<DiffHunk>{});
      }
      else
      {
        auto& existingHunks = (*fileHunksIt).second;
        existingHunks.push_back(hunk);
      }
    }
}

bool DiffParser::shouldNewFileBeProcessed(std::string_view line)
{
  std::string newFileTag{"+++ b/"};
  auto it = std::search(line.begin(), line.end(), newFileTag.begin(), newFileTag.end());
  if (it == line.begin()) { return true; }
  return false;
}

bool DiffParser::isFileRemoved(std::string_view line)
{
  std::string newFileTag{"+++ /dev/null"};
  auto it = std::search(line.begin(), line.end(), newFileTag.begin(), newFileTag.end());
  if (it == line.begin()) { return true; }
  return false;
}

bool DiffParser::isDiffTag(std::string_view line)
{
  auto const openTag  = las::DIFF_HUNK_TAG_OPEN;
  auto const closeTag = las::DIFF_HUNK_TAG_CLOSE;

  auto commaCount{0};
  auto itOpen  = std::search(line.begin(), line.end(), openTag.begin(), openTag.end());
  auto itClose = std::search(line.begin(), line.end(), closeTag.begin(), closeTag.end());

  if (itOpen == line.end() or itClose == line.end() or itOpen != line.begin()) { return false; }
  auto hasMinus{false}, hasPlus{false};
  std::for_each(itOpen, itClose, [&commaCount, &hasMinus, &hasPlus](auto const elem)
  {
    if (char(elem) == '-') { hasMinus = true; }
    else if (char(elem) == '+') { hasPlus = true; }
    else if (char(elem) == ',') { commaCount++; }
  });

  return hasMinus and hasPlus and (commaCount == 2);
}

std::string DiffParser::readFilename(std::string_view line)
{
  auto filenamePred{'/'};
  auto filenameIt = std::find(line.begin(), line.end(), filenamePred);
  std::string filename {filenameIt+1, line.end()};
  return filename;
}

}