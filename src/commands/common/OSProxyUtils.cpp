#include "OSProxyUtils.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include <vector>
#include <stdio.h>

namespace las::commands::common
{

void saveCommandResult(char const* cmd, std::string& result)
{
  std::array<char, 1024> line;
  std::unique_ptr<FILE, decltype(&pclose)> cmdPipe(popen(cmd, "r"), pclose);
  if (not cmdPipe) return;
  while (fgets(line.data(), line.size(), cmdPipe.get()) != nullptr)
  {
    result += line.data();
    std::cout<<line.data();
  }
}

void readFileContent(std::stringstream& contentStream, std::string const& filename)
{
  std::ifstream fileStream{filename};
  if(fileStream)
  {
    contentStream << fileStream.rdbuf();
    contentStream << "\n";
    fileStream.close();
  }
}

void writeContentToFile(std::stringstream& contentStream, std::string const& filename)
{
  std::ofstream outFileStream{filename};
  if(outFileStream)
  {
    outFileStream << contentStream.str();
    outFileStream.close();
  }
}

}
