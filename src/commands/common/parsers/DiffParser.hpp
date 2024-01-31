#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <optional>
#include <unordered_map>
#include "../Utils.hpp"

namespace las::commands::common
{

class ADiffParser
{
public:
  ADiffParser() = default;
  virtual ~ADiffParser() = default;
  virtual void parse(std::string_view content) = 0;
  virtual std::vector<DiffHunk>& getDiffHunks(const std::string& filename) = 0;
};

class DiffParser: public ADiffParser
{
public:
  DiffParser(std::vector<std::string>& filenames): filenames(filenames) {};
  void parse(std::string_view content) override;
  std::vector<DiffHunk>& getDiffHunks(std::string const& filename)
  {
    std::cout<<"RPY::getDiffHunks"<<std::endl;
    return diffHunks.at(filename);
  };

private:
  void setCurrentFilename(std::optional<std::string> filename);
  void createDiffHunkIfPossible(std::string_view line);
  bool shouldNewFileBeProcessed(std::string_view line);
  bool isFileRemoved(std::string_view line);
  bool isDiffTag(std::string_view line);
  std::string readFilename(std::string_view line);
  void removeFilename(std::string_view f);

  std::vector<std::string>& filenames;
  std::optional<std::string> currentFilename{std::nullopt};
  std::unordered_map<std::string, std::vector<DiffHunk>> diffHunks{};
};

}