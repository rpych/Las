#pragma once
#include "../parsers/HunksParser.hpp"

namespace las::commands::common
{

class ACodeWriter
{
public:
  using LasHunk = HunksParser::LasHunk;
  ACodeWriter() = default;
  virtual ~ACodeWriter() = default;
  virtual void write(std::vector<LasHunk> const&, std::vector<DiffHunk> const&) = 0;
};

}