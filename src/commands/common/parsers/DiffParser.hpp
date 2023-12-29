#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <optional>
#include <unordered_map>

namespace las::commands::common
{

class ADiffParser
{
public:
  ADiffParser() = default;
  virtual ~ADiffParser() = default;
  virtual void parse(std::string_view content) = 0;
};

class DiffParser: public ADiffParser
{
public:
  DiffParser() = default;
  void parse(std::string_view content) override;

  struct DiffHunk
  {
    uint64_t startLineOrig{};
    uint32_t numOfLinesOrig{};
    uint64_t startLineNew{};
    uint32_t numOfLinesNew{};
    std::string content{};
  };

private:
  void setCurrentFilename(std::optional<std::string> filename);
  void createDiffHunkIfPossible(std::string_view line);
  bool shouldNewFileBeProcessed(std::string_view line);
  bool isFileRemoved(std::string_view line);
  bool isDiffTag(std::string_view line);
  std::string readFilename(std::string_view line);

  std::optional<std::string> currentFilename{std::nullopt};
  std::unordered_map<std::string, std::vector<DiffHunk>> diffHunks{};
};

}