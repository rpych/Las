#pragma once
#include <cstdint>
//#include <optional>
#include <stack>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

#include <string>
#include "../../../Const.hpp"
#include "../Utils.hpp"
#include "LasParserSmManager.hpp"

namespace las::commands::common
{

// template<typename LANG>
// class LasParserSmManager;

class HunksParser
{
public:
  // enum class Comment
  // {
  //   OPENING,
  //   CLOSING
  // };

  // struct LasComment
  // {
  //   Comment comment{};
  //   uint64_t line{};
  // };

  // struct LasHunk
  // {
  //   std::string substContent{};
  //   LasComment opComment{};
  //   std::optional<LasComment> clComment{std::nullopt};
  //   bool contains(uint64_t lineNum) const
  //   {
  //     if (not clComment.has_value()) { return opComment.line == lineNum; }
  //     return lineNum >= opComment.line and lineNum <= clComment.value().line;
  //   }

  //   friend std::ostream& operator<<(std::ostream& out, LasHunk const& lh)
  //   {
  //     auto const clLine = (lh.clComment.has_value()) ? lh.clComment.value().line : std::numeric_limits<uint64_t>::max();
  //     return out<<"Opening::line="<<lh.opComment.line<<" closing::line="<<clLine<<"\nsubstContent\n"<<lh.substContent<<std::endl;
  //   }
  // };

  HunksParser() = default;
  void parseForHunks(std::stringstream& s);
  std::vector<LasHunk> getHunks() const { 
    std::cout<<"RPY::getHunks::"<<std::endl;
    return lpSm.hunks; };

private:
  void parseLasHunkIndicators(std::string_view line, uint64_t lineNum);
  LasParserSmManager<LAS_CPP> lpSm{};
  //bool expectSubstitutionCode{false};
  //std::stack<LasComment> commIndicators;
  //std::vector<LasHunk> hunks;
};

}
