#pragma once
#include <string>
#include <string_view>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <unordered_map>
#include "HunksParser.hpp"
#include "DiffParser.hpp"

namespace las::commands::common
{

class AFileParser
{
public:
  AFileParser() = default;
  virtual ~AFileParser() = default;
  virtual void parse(std::string_view diffContent, std::string_view filenamesBundle) = 0;
};

class FileParser: public AFileParser
{
public:
  FileParser();
  void parse(std::string_view diffContent, std::string_view filenamesBundle) override;
private:
  void setFilenames(std::string_view filenamesBundle);
  void parseFileStream(std::stringstream& s, std::string_view filename);

  using LasHunk = HunksParser::LasHunk;
  std::vector<std::string> filenames{};
  std::unordered_map<std::string, std::vector<LasHunk>> filesHunks{};
  std::unique_ptr<ADiffParser> diffParser{};
};

}