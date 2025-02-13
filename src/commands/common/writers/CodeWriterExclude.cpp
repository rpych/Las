#include "CodeWriterExclude.hpp"
#include "../OSProxyUtils.hpp"
#include "WriterUtils.hpp"
#include <string>
#include <iostream>
#include <sstream>

namespace las::commands::common
{

void CodeWriterExclude::write(std::string const& filename,
                              std::vector<LasHunk> const& lasHunks)
{
  readFileContent(inFileContentStream, filename);
  updateFileContent(filename, lasHunks);
  writeContentToFile(outFileContentStream, filename);
  inFileContentStream = std::stringstream{};
  outFileContentStream = std::stringstream{};
}

void CodeWriterExclude::updateFileContent(std::string const& filename,
                                          std::vector<LasHunk> const& lasHunks)
{
  logLasDebug("CodeWriterExclude::updateFileContent filename: {}", filename);
  std::shared_ptr<LasLanguage> lasLang{getLanguage(filename)};
  auto lineNum{1};
  std::string line{};
  while(std::getline(inFileContentStream, line))
  {
    auto matchingLasHunk = getLasHunkContainingLine(lasHunks, lineNum);
    if (not matchingLasHunk)
    {
      line = (isEmptyStream(outFileContentStream) ? "" : "\n") + line;
      outFileContentStream << line;
    }
    else if (matchingLasHunk and 
            (lasLang->containsLasIndSubstClose(line) or (lasLang->containsSingleLasInd(line) and matchingLasHunk->clComment.line == lineNum)))
    {
      auto& substitutionContent = matchingLasHunk->substContent;
      auto substContentUpdated = (substitutionContent != "") ? substitutionContent.substr(0, (substitutionContent.find_last_not_of("\n") + 1))
                                                             : substitutionContent;
      if (substContentUpdated != "")
      {
        outFileContentStream << (((isEmptyStream(outFileContentStream)) ? "" : "\n") + (substContentUpdated));
      }
    }
    lineNum++;
    line.clear();
  }
  outFileContentStream << "\n";
}

}