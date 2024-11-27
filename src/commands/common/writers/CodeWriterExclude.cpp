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
}

void CodeWriterExclude::updateFileContent(std::string const& filename,
                                          std::vector<LasHunk> const& lasHunks)
{
  auto containsLasIndSubstClose = [](std::string_view l) { return l.find("//^^las sub end^^") != std::string::npos; };
  auto lineNum{1};
  std::string line{};
  while(std::getline(inFileContentStream, line))
  {
    std::cout<<"::LINE to write: "<<line<<"myend"<<std::endl;
    auto matchingLasHunk = getLasHunkContainingLine(lasHunks, lineNum);
    if (not matchingLasHunk)
    {
      line = (lineNum == 1 ? "" : "\n") + line;
      outFileContentStream << line;
    }
    else if (containsLasIndSubstClose(line) and matchingLasHunk)
    {
      auto& substitutionContent = matchingLasHunk->substContent;
      auto substContentUpdated = substitutionContent.substr(0, substitutionContent.length()-1);
      outFileContentStream << ((lineNum == 1 ? "" : "\n") + (substContentUpdated));
    }
    lineNum++;
    line.clear();
  }
}

}