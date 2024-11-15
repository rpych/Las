#pragma once
#include <vector>
#include <optional>
#include "../Utils.hpp"
#include "../parsers/HunksParser.hpp"

namespace las::commands::common
{

inline bool anyLasHunkContainsLine(std::vector<HunksParser::LasHunk> const& lasHunks, uint64_t lineNum)
{
  auto res = std::find_if(lasHunks.begin(), lasHunks.end(), [lineNum](auto const& hunk)
  {
    return hunk.contains(lineNum);
  });
  return res != lasHunks.end();
}
}

//^^las begin^^
int fun()
{
  double c = 0.2;
  return c;
}
//^^las end^^