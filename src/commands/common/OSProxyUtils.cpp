#include "OSProxyUtils.hpp"
#include <iostream>

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
  std::cout<<"SIZE of the output string: "<<result.size()<<", length: "<<result.length()<<std::endl;
}

}
