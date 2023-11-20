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
  virtual void executeOsCommand(T command) = 0;
  virtual std::string& getOsCommandResult() = 0;
  virtual ~AOSCommandProxy() = default;
};

template<typename T>
class OSCommandProxy: public AOSCommandProxy<T>
{
public:
  OSCommandProxy(): allowedOsCommands{initAllowedOsCommands()} {};
  void executeOsCommand(T command) override;
  std::string& getOsCommandResult() override;
private:
  std::map<T, std::string> const initAllowedOsCommands();
  std::map<T, std::string> const& getAllowedOsCommands();
  std::map<T, std::string> const allowedOsCommands{};
  std::string osCommandOutput{}; 
};

}
