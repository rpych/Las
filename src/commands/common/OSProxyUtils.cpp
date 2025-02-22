#include "OSProxyUtils.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../../Const.hpp"

namespace las::commands::common
{

void saveCommandResult(char const* cmd, std::string& result)
{
  std::array<char, 1024> line;
  auto pcloseWrap = [](FILE* f) { return pclose(f); };
  std::unique_ptr<FILE, decltype(pcloseWrap)> cmdPipe(popen(cmd, "r"), pcloseWrap);
  if (not cmdPipe) return;
  while (fgets(line.data(), line.size(), cmdPipe.get()) != nullptr)
  {
    result += line.data();
  }
}

void executeCommand(char const* cmd)
{
  logLasDebug("executeCommand: {}", cmd);
  FILE* f = popen(cmd, "w");
  if (f)
  {
    fputs(" ", f);
  }
  pclose(f);
}

void readFileContent(std::stringstream& contentStream, std::string const& filename)
{
  logLasDebug("readFileContent: {}", filename);
  std::ifstream fileStream{filename};
  if(fileStream)
  {
    std::string line{};
    while(std::getline(fileStream, line))
    {
      contentStream << (line + "\n");
    }
    fileStream.close();
  }
  else
  {
    logLasError("Error: file {} not found when trying to read", filename);
  }
}

void writeContentToFile(std::stringstream& contentStream, std::string const& filename)
{
  logLasDebug("writeContentToFile: {}", filename);
  std::ofstream outFileStream{filename};
  if(outFileStream)
  {
    outFileStream << contentStream.str();
    outFileStream.close();
  }
  else
  {
    logLasError("Error: file {} not found when trying to write", filename);
  }
}

}
