#pragma once
#include "ACodeWriter.hpp"

namespace las::commands::common
{
 
class CodeWriterExclude: public ACodeWriter
{
public:
  void write(std::string const& filename, std::vector<LasHunk> const& lasHunks) override;
private:
  void updateFileContent(std::string const& filename, std::vector<LasHunk> const& lasHunks);
};


}