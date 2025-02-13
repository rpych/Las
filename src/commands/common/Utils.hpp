#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <optional>
#include <algorithm>
#include <memory>
#include <utility>
#include <limits>
#include <sstream>
#include <iterator>
#include <iostream>
#include <sys/stat.h>
#include "../../Const.hpp"

namespace las::commands::common
{

inline bool rdRestoreEnabled{true};

enum class RepoState
{
  SAVE,
  ROLLBACK
};

enum class LasCmd
{
  INVALID,
  CUT,
  RESTORE,
  DIFF,
  DIFF_HEAD,
  DIFF_STAGED
};

enum class LasCmdOpts
{
  INVALID,
  FILES,
  SINGLE_MODE,
  PARALLEL_MODE,
  LIST,
  SHOW
};

enum class GitCmd
{
  GIT_DIFF_FILES,
  GIT_DIFF,
  GIT_DIFF_HEAD_FILES,
  GIT_DIFF_HEAD,
  GIT_DIFF_STAGED_FILES,
  GIT_DIFF_STAGED,
  GIT_STASH,
  GIT_STASH_PUSH_STAGED,
  GIT_STASH_PUSH_KEEP_INDEX,
  GIT_STASH_APPLY,
  GIT_STASH_APPLY_1,
  GIT_STASH_POP,
  GIT_STASH_POP_INDEX,
  GIT_STASH_POP_INDEX_1,
  GIT_RESET_HARD,
  GIT_STASH_DROP,
  GIT_STASH_DROP_1,
  GIT_STATUS,
  GIT_APPLY_INDEX,
  GIT_APPLY
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
  LasComment clComment{};
  bool contains(uint64_t lineNum) const
  {
    return lineNum >= opComment.line and lineNum <= clComment.line;
  }

  friend std::ostream& operator<<(std::ostream& out, LasHunk const& lh)
  {
    return out<<"Opening::line="<<lh.opComment.line<<" closing::line="<<lh.clComment.line<<"\nsubstContent\n"<<lh.substContent<<std::endl;
  }
};

inline bool fileExists(std::string const& filename)
{
  struct stat st;
  return (stat(filename.c_str(), &st) == 0);
}

inline bool isReprNumber(std::string_view lexem)
{
  return not lexem.empty() and 
         std::find_if(lexem.begin(), lexem.end(), [](auto s) { return not std::isdigit(s); }) == lexem.end();
}

static std::map<Language, std::shared_ptr<LasLanguage>> predefineLanguages()
{
  std::map<Language, std::shared_ptr<LasLanguage>> opts;
  opts.emplace(Language::CPP, std::make_shared<LasCpp>());
  return opts;
}

static std::map<Language, std::shared_ptr<LasLanguage>> availableLanguages {predefineLanguages()};


inline std::vector<std::string> const getFilenamesFromStatusCmd(std::string const& filenamesBundle)
{
  std::stringstream filenamesStream{filenamesBundle};
  std::vector<std::string> filenames{};
  std::string filename{};
  while(std::getline(filenamesStream, filename))
  {
    if(not filename.empty()) { filenames.push_back(filename); }
    filename.clear();
  }
  return filenames;
}

inline std::string const getParsedFileExtension(std::string_view filename)
{
  auto const fileExtensionIdx{filename.find_last_of(".")};
  if (fileExtensionIdx == std::string::npos)
  {
    logLasError("Error::file: {} without extension!", filename);
  }
  auto const fileExtension{filename.substr(fileExtensionIdx+1)};
  return static_cast<std::string>(fileExtension);
}

inline std::shared_ptr<LasLanguage> getLanguage(std::string_view filename)
{
  auto const extension{getParsedFileExtension(filename)};
  if (extension == "cpp" or extension == "hpp")
  {
    return availableLanguages.at(Language::CPP);
  }
  return std::shared_ptr<LasLanguage>();
}

inline std::string const getSubstContentFromRightSide(std::string_view line, std::string_view lasKeyPhrase)
{
  auto const markEndIdx{line.find(lasKeyPhrase) + lasKeyPhrase.length()};
  auto const linePart{line.substr(markEndIdx)};
  auto const lineEndIdx{linePart.find_last_not_of(" \n\t")};
  std::string substContent{linePart.substr(0, (lineEndIdx != std::string::npos ? lineEndIdx+1 : 0))};
  substContent = (substContent.length() == 0 ? substContent : (substContent + "\n"));
  return substContent;
}

inline std::string const getSubstContentFromLeftSide(std::string_view line, std::string_view lasKeyPhrase, std::string_view langComment)
{
  auto const markStartIdx{line.find(lasKeyPhrase)};
  auto const linePart{line.substr(0, markStartIdx)};
  auto const lineEndIdx{linePart.find_last_not_of(langComment)};
  std::string substContent{linePart.substr(0, (lineEndIdx != std::string::npos ? lineEndIdx+1 : 0))};
  substContent = (substContent.find_last_not_of(" \n\t") != std::string::npos) ? substContent : "";
  return substContent;
}

inline std::string const getNormalLineSubstContent(std::string_view line, std::string_view langComment)
{
  auto const foundCommentStartIdx{line.find(langComment)};
  auto const notBlankStartIdx{line.find_first_not_of(" \n\t")};
  if ((foundCommentStartIdx != std::string::npos) and foundCommentStartIdx == notBlankStartIdx)
  {
    std::string const substContentLeft {line.substr(0, foundCommentStartIdx)};
    std::string const substContentRight {line.substr(foundCommentStartIdx + langComment.length())};
    return (substContentLeft + substContentRight);
  }
  return static_cast<std::string>(line);
}

}
