#pragma once

namespace las::commands::common
{

class ACodeWriter
{
public:
  ACodeWriter() = default;
  virtual ~ACodeWriter() = default;
  virtual void write() = 0;
};

}