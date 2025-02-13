#pragma once
#include <vector>
#include <algorithm>
#include <optional>
#include "../Utils.hpp"

namespace las::commands::common
{

inline std::optional<LasHunk> getLasHunkContainingLine(std::vector<LasHunk> const& lasHunks, uint64_t lineNum)
{
  auto res = std::find_if(lasHunks.begin(), lasHunks.end(), [lineNum](auto const& hunk)
  {
    return hunk.contains(lineNum);
  });
  return (res != lasHunks.end() ? *res : std::optional<LasHunk>(std::nullopt));
}

inline bool isEmptyStream(std::stringstream& stream)
{
  return stream.tellp() == 0;
} 

}
