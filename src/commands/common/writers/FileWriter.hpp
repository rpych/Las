#pragma once
#include <memory>
#include "../Utils.hpp"

namespace las::commands::common
{
class ACodeWriter;
class AFileParser;

class AFileWriter
{
public:
  AFileWriter() = default;
  virtual ~AFileWriter() = default;
  virtual void write(std::unique_ptr<AFileParser> fileParser) = 0;
};

class FileWriter: public AFileWriter
{
public:
  FileWriter(LasCmdOpts opt): option(opt) {}
  void write(std::unique_ptr<AFileParser> fileParser) override;
private:
  LasCmdOpts option{};
  std::unique_ptr<ACodeWriter> createCodeWriter(LasCmdOpts option);
};

}