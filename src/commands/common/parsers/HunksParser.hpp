#pragma once
#include <cstdint>
#include <optional>
#include <stack>
#include <vector>
#include <sstream>
#include <iostream>
#include <limits>


namespace las::commands::common
{

class HunksParser
{
public:
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

    friend std::ostream& operator<<(std::ostream& out, LasHunk const& lh)
    {
      auto const clLine = (lh.clComment.has_value()) ? lh.clComment.value().line : std::numeric_limits<uint64_t>::max();
      return out<<"Opening::line="<<lh.opComment.line<<" closing::line="<<clLine<<std::endl;
    }
  };

  HunksParser() = default;
  void parseForHunks(std::stringstream& s);
  std::vector<LasHunk> getHunks() const { return hunks; };

private:
  std::stack<LasComment> commIndicators;
  std::vector<LasHunk> hunks;
};

}