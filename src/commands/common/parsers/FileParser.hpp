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

namespace las::commands::common
{

class AFileParser
{
public:
  using LasHunk = HunksParser::LasHunk;
  AFileParser() = default;
  virtual ~AFileParser() = default;
  virtual void parse(std::vector<std::string>& filenames) = 0;
  virtual std::unordered_map<std::string, std::vector<LasHunk>> const& getFilesHunks() = 0;
};

class FileParser: public AFileParser
{
public:
  FileParser();
  void parse(std::vector<std::string>& filenames) override;
  std::unordered_map<std::string, std::vector<LasHunk>> const& getFilesHunks() override
  {
    return filesHunks;
  }
private:
  void parseFileStream(std::stringstream& s, std::string_view filename);
  std::unordered_map<std::string, std::vector<LasHunk>> filesHunks{};

};

}
