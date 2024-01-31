#include "CodeWriterExclude.hpp"
#include <iostream>

namespace las::commands::common
{

void CodeWriterExclude::write(std::vector<LasHunk> const&, std::vector<DiffHunk> const&)
{
  std::cout<<"RPY::CodeWriterExclude::write"<<std::endl;
}

}