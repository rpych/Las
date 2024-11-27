#include "HunksParser.hpp"

namespace las::commands::common
{

void HunksParser::parseForHunks(std::stringstream& s)
{
  uint64_t lineNum{1};
  while(s)
  {
    std::string line{};
    std::getline(s, line);
    line += "\n";
    parseLasHunkIndicators(line, lineNum);
    lineNum++;
  }
}

void HunksParser::parseLasHunkIndicators(std::string_view line, uint64_t lineNum)
{
  auto containsLasIndOpen   = [](std::string_view l) { return l.find("//^^las begin^^") != std::string::npos; };
  auto containsLasIndClose  = [](std::string_view l) { return l.find("//^^las end^^") != std::string::npos; };
  auto containsSingleLasInd = [](std::string_view l) { return l.find("//^^las^^") != std::string::npos; };
  auto containsLasIndSubstOpen  = [](std::string_view l) { return l.find("//^^las sub begin^^") != std::string::npos; };
  auto containsLasIndSubstClose = [](std::string_view l) { return l.find("//^^las sub end^^") != std::string::npos; };

  if (containsSingleLasInd(line))
  {
    std::cout<<"parseForHunks::containsSingleLasInd with"<<line<<std::endl;
    hunks.push_back(LasHunk{.isBlockHunk=false, .opComment=LasComment{Comment::OPENING, lineNum}});
  }
  else if (containsLasIndOpen(line))
  {
    std::cout<<"parseForHunks::containsLasIndOpen with"<<line<<std::endl;
    commIndicators.push(LasComment{Comment::OPENING, lineNum});
  }
  else if (containsLasIndClose(line))
  {
    std::cout<<"parseForHunks::containsLasIndClose with "<<line<<std::endl;
    if (commIndicators.empty())
    {
      std::cout<<"Any opening LAS comment does not exist in the file"<<std::endl;
      return;
    }
    auto const& openingComment = commIndicators.top();
    commIndicators.pop();
    if (openingComment.comment != Comment::OPENING)
    {
      std::cout<<"Lacking LAS opening comment for LAS closing comment"<<std::endl;
      return;
    }
    hunks.push_back(LasHunk{.isBlockHunk=true, .opComment=openingComment, .clComment=std::make_optional(LasComment{Comment::CLOSING, lineNum})});
  }
  else if(containsLasIndSubstOpen(line))
  {
    expectSubstitutionCode = true;
  }
  else if(containsLasIndSubstClose(line))
  {
    if (not expectSubstitutionCode)
    {
      std::cout<<"Substitution las closing mark not expected"<<std::endl;
      return;
    }
    auto& lastHunk = hunks.back();
    lastHunk.clComment->line = lineNum;
    expectSubstitutionCode = false;
  }
  else if (expectSubstitutionCode)
  {
    auto& lastHunk = hunks.back();
    lastHunk.substContent += line;
  }
}

}
