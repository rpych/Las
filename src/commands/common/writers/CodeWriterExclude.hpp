#pragma once
#include "ACodeWriter.hpp"

namespace las::commands::common
{
 
class CodeWriterExclude: public ACodeWriter
{
public:
  void write() override;
};


}