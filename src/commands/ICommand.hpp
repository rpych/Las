#pragma once


namespace las::commands
{

class ICommand
{
public:
  virtual void runAlgorithm() = 0;
  virtual ~ICommand() = default;
};

}
