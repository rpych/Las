#pragma once
#include "ACodeWriter.hpp"

namespace las::commands::common
{
 
class CodeWriterExclude: public ACodeWriter
{
public:
  void write(std::vector<LasHunk> const&, std::vector<DiffHunk> const&) override;
};


}