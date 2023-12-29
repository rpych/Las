#include "HunksParser.hpp"

namespace las::commands::common
{

void HunksParser::parseForHunks(std::stringstream& s)
{
  auto containsLasIndOpen =  [](std::string_view l) { return l.find("//^^las begin^^") != std::string::npos; };
  auto containsLasIndClose = [](std::string_view l) { return l.find("//^^las end^^") != std::string::npos; };
  uint64_t lineNum{0};
  while(s)
  {
    std::string line{};
    s >> line;
    if (containsLasIndOpen(line))
    {
      std::cout<<"parseForHunks::containsLasIndOpen with %s"<<line<<std::endl;
      commIndicators.push(LasComment{Comment::OPENING, lineNum});
    }
    else if (containsLasIndClose(line))
    {
      std::cout<<"parseForHunks::containsLasIndClose with %s"<<line<<std::endl;
      auto const& openingComment = commIndicators.top();
      commIndicators.pop();
      hunks.push_back(LasHunk{true, openingComment, std::make_optional(LasComment{Comment::CLOSING, lineNum})});
    }
    lineNum++;
  }
}

}