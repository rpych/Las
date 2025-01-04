#pragma once
#include <string>
#include <memory>
#include "../ui/CmdParser.hpp"
#include "../commands/CommandExecutor.hpp"

// namespace las::commands
// {
//   class ICommandExecutor;
// }

namespace las::executors
{

class ILas
{
public:
  virtual void run(char const* args[]) = 0;
  virtual ~ILas() = default;
};

class Las: public ILas
{
public:
  Las(int argc);
  void run(char const* args[]) override;
private:
  std::unique_ptr<las::ui::ICmdParser> parser;
  std::unique_ptr<las::commands::ICommandExecutor> cmdExecutor;
};
}
