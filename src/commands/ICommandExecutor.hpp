#pragma once

namespace las::commands
{

class ICommandExecutor
{
public:
  virtual void execute() = 0;
};

}