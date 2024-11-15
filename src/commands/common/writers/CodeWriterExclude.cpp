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
  auto lineNum{1};
  std::string line{};
  while(std::getline(inFileContentStream, line))
  {
    std::cout<<"::LINE to write: "<<line<<"myend"<<std::endl;
    if (not anyLasHunkContainsLine(lasHunks, lineNum))
    {
      line = (lineNum == 1 ? "" : "\n") + line;
      outFileContentStream << line;
    }
    lineNum++;
    line.clear();
  }
}

}