#pragma once
#include <cstdint>
#include <optional>
#include <stack>
#include <vector>
#include <sstream>
#include <iostream>


namespace las::commands::common
{

class HunksParser
{
public:
  HunksParser() = default;
  void parseForHunks(std::stringstream& s);

  enum class Comment
  {
    OPENING,
    CLOSING
  };

  struct LasComment
  {
    Comment comment{};
    uint64_t line{};
  };

  struct LasHunk
  {
    bool isBlockHunk{};
    LasComment opComment{};
    std::optional<LasComment> clComment{std::nullopt};
  };

private:
  std::stack<LasComment> commIndicators;
  std::vector<LasHunk> hunks;
};

}