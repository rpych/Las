#pragma once
#include <string>
#include <sstream>
#include "../Utils.hpp"

namespace las::commands::common
{

class ACodeWriter
{
public:
  ACodeWriter() = default;
  virtual ~ACodeWriter() = default;
  virtual void write(std::string const& filename, std::vector<LasHunk> const& lasHunks) = 0;
protected:
  std::stringstream inFileContentStream{}; 
  std::stringstream outFileContentStream{};

};

}