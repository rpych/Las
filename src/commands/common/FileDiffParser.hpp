#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

namespace las::commands::common
{

class AFileDiffParser
{
public:
  AFileDiffParser() = default;
  virtual ~AFileDiffParser() = default;
  virtual void parse(std::string_view filenamesBundle) = 0;
};

class FileDiffParser: public AFileDiffParser
{
public:
  FileDiffParser();
  void parse(std::string_view filenamesBundle) override;
private:
  void setFilenames(std::string_view filenamesBundle);
  std::vector<std::string> filenames{};
};

}