#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <optional>
#include <utility>
#include <limits>
#include <sstream>
#include <iterator>
#include <iostream>

namespace las::commands::common
{

enum class LasCmd
{
  INVALID,
  CUT,
  DIFF
};

enum class LasCmdOpts
{
  INVALID,
  EXCLUDE,
  INCLUDE
};

enum class GitCmd
{
  GIT_STATUS,
  GIT_DIFF_HEAD,
  GIT_STASH,
  GIT_STASH_APPLY,
  GIT_STASH_POP,
  GIT_RESET_HARD
};

enum class Comment
  {
    OPENING,
    CLOSING
  };

struct LasComment
{
  Comment comment{};
  uint64_t line{};
};

struct LasHunk
{
  std::string substContent{};
  LasComment opComment{};
  std::optional<LasComment> clComment{std::nullopt};
  bool contains(uint64_t lineNum) const
  {
    if (not clComment.has_value() ) { return opComment.line == lineNum; }
    return lineNum >= opComment.line and lineNum <= clComment.value().line;
  }

  friend std::ostream& operator<<(std::ostream& out, LasHunk const& lh)
  {
    auto const clLine = (lh.clComment.has_value()) ? lh.clComment.value().line : std::numeric_limits<uint64_t>::max();
    return out<<"Opening::line="<<lh.opComment.line<<" closing::line="<<clLine<<"\nsubstContent\n"<<lh.substContent<<std::endl;
  }
};

static std::map<LasCmd, std::set<LasCmdOpts>> predefineOpts()
{
  std::map<LasCmd, std::set<LasCmdOpts>> opts;
  opts.emplace(LasCmd::INVALID, std::set({LasCmdOpts::INVALID}));
  opts.emplace(LasCmd::CUT, std::set({LasCmdOpts::EXCLUDE}));
  opts.emplace(LasCmd::DIFF, std::set({LasCmdOpts::EXCLUDE, LasCmdOpts::INCLUDE}));
  return opts;
}

static std::map<LasCmd, std::set<LasCmdOpts>> availableOpts {predefineOpts()};


inline std::vector<std::string> const getFilenamesFromStatusCmd(std::string_view filenamesBundle)
{
  std::stringstream filenamesStream{};
  filenamesStream << filenamesBundle;
  std::vector<std::string> filenames{};
  while(filenamesStream)
  {
    std::string filename{};
    filenamesStream >> filename;
    if(not filename.empty()) { filenames.push_back(filename); }
  }
  std::copy(filenames.begin(), filenames.end(), std::ostream_iterator<std::string>(std::cout, ",,"));
  return filenames;
}

//template<const char* pattern>
inline bool containsLasIndOpen(std::string_view l) { return l.find("//^^las begin^^") != std::string::npos; }
inline bool containsLasIndClose(std::string_view l) { return l.find("//^^las end^^") != std::string::npos; }
inline bool containsSingleLasInd(std::string_view l) { return l.find("//^^las^^") != std::string::npos; }
inline bool containsLasIndSubstOpen(std::string_view l) { return l.find("//^^las sub begin^^") != std::string::npos; }
inline bool containsLasIndSubstClose(std::string_view l) { return l.find("//^^las sub end^^") != std::string::npos; }
}
