#include "OSCommandProxy.hpp"

namespace las::commands::common
{

template<typename GitCmd>
std::map<GitCmd, std::string> const OSCommandProxy<GitCmd>::initAllowedOsCommands()
{
  using namespace std::string_literals;
  std::map<GitCmd, std::string> osCommands;
  osCommands.emplace(GitCmd::GIT_STATUS, "git diff --name-only "s);
  osCommands.emplace(GitCmd::GIT_DIFF, "git diff"s);
  osCommands.emplace(GitCmd::GIT_STASH_PUSH, "git stash push"s);
  osCommands.emplace(GitCmd::GIT_STASH_POP, "git stash pop"s);
  return osCommands;
}

template<typename T>
std::map<T, std::string> const& OSCommandProxy<T>::getAllowedOsCommands()
{
  return allowedOsCommands;
}

template<typename T>
std::string& OSCommandProxy<T>::getOsCommandResult()
{
  return osCommandOutput;
}

template<typename T>
void OSCommandProxy<T>::clearOsCommand()
{
  osCommandOutput.clear();
}

template<typename T>
void OSCommandProxy<T>::executeOsCommand(T command)
{
  namespace osUtils = las::commands::common;
  clearOsCommand();
  std::string const& s  = static_cast<std::string>(allowedOsCommands.at(command));
  char const* cmdPhrase = s.c_str();
  osUtils::saveCommandResult(cmdPhrase, osCommandOutput);
}



template class OSCommandProxy<GitCmd>;

}
