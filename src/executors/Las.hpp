#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../ui/CmdParser.hpp"
#include "../commands/CommandExecutor.hpp"

namespace las::executors
{

class ILas
{
public:
  virtual void run(std::vector<std::string> const& args) = 0;
  virtual ~ILas() = default;
};

class Las: public ILas
{
public:
  Las(int argc);
  Las(int argc, std::unique_ptr<las::commands::ICommandExecutor> ce);
  void run(std::vector<std::string> const& args) override;
private:
  std::unique_ptr<las::ui::ICmdParser> parser;
  std::unique_ptr<las::commands::ICommandExecutor> cmdExecutor;
};
}
