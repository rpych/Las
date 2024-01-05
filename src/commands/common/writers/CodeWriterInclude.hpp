#pragma once
#include "ACodeWriter.hpp"

namespace las::commands::common
{

class CodeWriterInclude: public ACodeWriter
{
public:
  void write() override;
};

}