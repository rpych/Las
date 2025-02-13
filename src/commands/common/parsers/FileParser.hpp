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
#include "../Utils.hpp"
#include "HunksParser.hpp"

namespace las::commands::common
{

class AFileParser
{
public:
  AFileParser() = default;
  virtual ~AFileParser() = default;
  virtual bool parse(std::vector<std::string> const& filenames) = 0;
  virtual std::unordered_map<std::string, std::vector<LasHunk>> const& getFilesHunks() = 0;
};

class FileParser: public AFileParser
{
public:
  FileParser();
  bool parse(std::vector<std::string> const& filenames) override;
  std::unordered_map<std::string, std::vector<LasHunk>> const& getFilesHunks() override
  {
    return filesHunks;
  }
private:
  bool parseFileStream(std::stringstream& s, std::string_view filename);
  std::unordered_map<std::string, std::vector<LasHunk>> filesHunks{};

};

}
