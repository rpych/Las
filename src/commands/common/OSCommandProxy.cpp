#include "OSCommandProxy.hpp"

namespace las::commands::common
{

template<typename GitCmd>
std::map<GitCmd, std::string> const OSCommandProxy<GitCmd>::initAllowedOsCommands()
{
  using namespace std::string_literals;
  std::map<GitCmd, std::string> osCommands;
  osCommands.emplace(GitCmd::GIT_DIFF_FILES, "git diff --name-only"s);
  osCommands.emplace(GitCmd::GIT_DIFF, "git diff"s);
  osCommands.emplace(GitCmd::GIT_DIFF_HEAD_FILES, "git diff HEAD --name-only"s);
  osCommands.emplace(GitCmd::GIT_DIFF_HEAD, "git diff HEAD"s);
  //osCommands.emplace(GitCmd::GIT_DIFF_STAGED_FILES, "git diff --staged --name-only"s);
  //osCommands.emplace(GitCmd::GIT_DIFF_STAGED, "git diff --staged"s);
  osCommands.emplace(GitCmd::GIT_STASH, "git stash"s); // >/dev/null 2>&1
  osCommands.emplace(GitCmd::GIT_STASH_PUSH_STAGED, "git stash push --staged"s); // >/dev/null 2>&1
  osCommands.emplace(GitCmd::GIT_STASH_APPLY, "git stash apply --index stash@{0}"s);
  osCommands.emplace(GitCmd::GIT_STASH_APPLY_1, "git stash apply --index stash@{1}"s);
  osCommands.emplace(GitCmd::GIT_STASH_POP, "git stash pop"s);
  osCommands.emplace(GitCmd::GIT_STASH_POP_INDEX, "git stash pop --index"s);
  osCommands.emplace(GitCmd::GIT_STASH_POP_INDEX_1, "git stash pop --index stash@{1}"s);
  osCommands.emplace(GitCmd::GIT_RESET_HARD, "git reset HEAD --hard"s);
  osCommands.emplace(GitCmd::GIT_STASH_DROP, "git stash drop stash@{0}"s);
  osCommands.emplace(GitCmd::GIT_STASH_DROP_1, "git stash drop stash@{1}"s);
  return osCommands;
}

template<typename T>
std::map<T, std::string> const& OSCommandProxy<T>::getAllowedOsCommands()
{
  return allowedOsCommands;
}

template<typename T>
std::string const& OSCommandProxy<T>::getOsCommandResult()
{
  return osCommandOutput;
}

template<typename T>
void OSCommandProxy<T>::clearOsCommand()
{
  osCommandOutput.clear();
}

template<typename T>
std::string const& OSCommandProxy<T>::executeOsCommand(T command)
{
  namespace osUtils = las::commands::common;
  clearOsCommand();
  std::string const& s  = static_cast<std::string>(allowedOsCommands.at(command));
  char const* cmdPhrase = s.c_str();
  osUtils::saveCommandResult(cmdPhrase, osCommandOutput);
  std::string const& cmdResult = getOsCommandResult();
  return cmdResult;
}

template<typename T>
void OSCommandProxy<T>::executeOsCommandNotSave(T command)
{
  namespace osUtils = las::commands::common;
  clearOsCommand();
  std::string const& s  = static_cast<std::string>(allowedOsCommands.at(command));
  char const* cmdPhrase = s.c_str();
  osUtils::executeCommand(cmdPhrase);
}



template class OSCommandProxy<GitCmd>;

}
