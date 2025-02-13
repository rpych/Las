#include "HunksParser.hpp"

namespace las::commands::common
{

bool HunksParser::parseForHunks(std::stringstream& s)
{
  uint64_t lineNum{1};
  std::string line{};
  while(std::getline(s, line))
  {
    line += "\n";
    if (not lpSm.processLine(line, lineNum)) { return false; }
    lineNum++;
    line.clear();
  }
  return true;
}

}
