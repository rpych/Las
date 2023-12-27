#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <optional>

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

  enum class Comment
  {
    OPENING,
    CLOSING
  };

  struct LasComment
  {
    Comment comment{};
    uint32_t line{};
  };

  struct LasHunk
  {
    bool isBlockHunk{};
    Comment opComment{};
    std::optional<Comment> clComment{std::nullopt};
  };

private:
  void setFilenames(std::string_view filenamesBundle);
  void parseFileStream(std::stringstream& s);

  std::vector<std::string> filenames{};
  std::unordered_map<std::string, std::vector<LasHunk>> filesHunks{};
};

}