#pragma once
#include <memory>
#include <unordered_map>
#include "../parsers/HunksParser.hpp"
#include "../Utils.hpp"

namespace las::commands::common
{
class ACodeWriter;
class AFileParser;

class AFileWriter
{
public:
  using LasHunk = HunksParser::LasHunk;
  AFileWriter() = default;
  virtual ~AFileWriter() = default;
  virtual void write(std::unordered_map<std::string, std::vector<LasHunk>>const& filesHunks) = 0;
};

class FileWriter: public AFileWriter
{
public:
  FileWriter(LasCmdOpts opt): option(opt) {}
  void write(std::unordered_map<std::string, std::vector<LasHunk>>const& filesHunks) override;
private:
  LasCmdOpts option{};
  std::unique_ptr<ACodeWriter> createCodeWriter(LasCmdOpts option);
};

}