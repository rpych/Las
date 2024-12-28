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
  //virtual std::string& getOsCommandResult() = 0;
  virtual ~AOSCommandProxy() = default;
};

template<typename T>
class OSCommandProxy: public AOSCommandProxy<T>
{
public:
  OSCommandProxy(): allowedOsCommands{initAllowedOsCommands()} {};
  std::string const& executeOsCommand(T command) override;
  void executeOsCommandNotSave(T command) override;
private:
  std::string const& getOsCommandResult();
  void clearOsCommand();
  std::map<T, std::string> const initAllowedOsCommands();
  std::map<T, std::string> const& getAllowedOsCommands();
  std::map<T, std::string> const allowedOsCommands{};
  std::string osCommandOutput{}; 
};

}
