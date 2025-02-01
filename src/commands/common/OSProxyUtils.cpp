#include "OSProxyUtils.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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

void executeCommand(char const* cmd)
{
  std::cout<<"executeCommand: "<<cmd<<std::endl;
  system(cmd);
}

void readFileContent(std::stringstream& contentStream, std::string const& filename)
{
  std::ifstream fileStream{filename};
  //std::cout<<"readFileContent file:"<<filename<<std::endl;
  if(fileStream)
  {
    //contentStream << fileStream.rdbuf();
    std::string line{};
    while(std::getline(fileStream, line))
    {
      //std::cout<<"readFileContent line:"<<line<<std::endl;
      contentStream << (line + "\n");
    }
    fileStream.close();
  }
  else
  {
    std::cout<<"ERR::readFileContent file "<<filename<<" not found"<<std::endl;
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
  else
  {
    std::cout<<"ERR::writeContentToFile file "<<filename<<" not found"<<std::endl;
  }
}

}
