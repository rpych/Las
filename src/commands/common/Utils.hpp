#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <sstream>

//temp
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

// struct DiffHunk
// {
//   uint64_t startLineOrig{};
//   uint32_t numOfLinesOrig{};
//   uint32_t startLineNew{};// TODO: remove
//   uint32_t numOfLinesNew{};// TODO: remove
// };

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
  filenamesStream << filenamesBundle;//test2
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

}
