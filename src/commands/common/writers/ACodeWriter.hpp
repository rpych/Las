#pragma once
#include <string>
#include <sstream>
#include "../parsers/HunksParser.hpp"

namespace las::commands::common
{

class ACodeWriter
{
public:
  using LasHunk = HunksParser::LasHunk;
  ACodeWriter() = default;
  virtual ~ACodeWriter() = default;
  virtual void write(std::string const& filename, std::vector<LasHunk> const& lasHunks) = 0;
protected:
  std::stringstream inFileContentStream{}; 
  std::stringstream outFileContentStream{};

};

}