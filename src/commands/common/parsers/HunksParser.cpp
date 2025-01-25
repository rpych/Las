#include "HunksParser.hpp"

namespace las::commands::common
{

void HunksParser::parseForHunks(std::stringstream& s)
{
  uint64_t lineNum{1};
  std::string line{};
  while(std::getline(s, line))
  {
    line += "\n";
    if (not lpSm.processLine(line, lineNum)) { return; }
    lineNum++;
    line.clear();
  }
}

}
