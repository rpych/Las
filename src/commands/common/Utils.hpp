#pragma once
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <sstream>

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
  GIT_DIFF,
  GIT_STASH_PUSH,
  GIT_STASH_POP
};

struct DiffHunk
{
  uint64_t startLineOrig{};
  uint32_t numOfLinesOrig{};
  uint64_t startLineNew{};
  uint32_t numOfLinesNew{};
  std::string content{};
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
  return filenames;
}

}
