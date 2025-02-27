#pragma once
#include <string>
#include <string_view>
#include <map>
#include "Utils.hpp"
#include "OSProxyUtils.hpp"

namespace las::commands::common
{

template<typename T>
class AOSCommandProxy
{
public:
  virtual std::string const& executeOsCommand(T command) = 0;
  virtual void executeOsCommandNotSave(T command) = 0;
  virtual void executeOsCommandNotSave(T command,
                                       std::vector<std::string> const& files,
                                       std::string const& outputFile) = 0;
  virtual void executeOsCommandWithFile(T command,
                                        std::string const& file,
                                        RepoState state) = 0;
  virtual ~AOSCommandProxy() = default;
};

template<typename T>
class OSCommandProxy: public AOSCommandProxy<T>
{
public:
  OSCommandProxy(): allowedOsCommands{initAllowedOsCommands()} {};
  std::string const& executeOsCommand(T command) override;
  void executeOsCommandNotSave(T command) override;
  virtual void executeOsCommandNotSave(T command,
                                       std::vector<std::string> const& files,
                                       std::string const& outputFile) override;
  virtual void executeOsCommandWithFile(T command,
                                        std::string const& file,
                                        RepoState state) override;
private:
  std::string const& getOsCommandResult();
  void clearOsCommand();
  std::map<T, std::string> const initAllowedOsCommands();
  std::map<T, std::string> const& getAllowedOsCommands();
  std::map<T, std::string> const allowedOsCommands{};
  std::string osCommandOutput{}; 
};

}
