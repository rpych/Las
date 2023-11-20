#pragma once
#include "ICommand.hpp"

namespace las::commands
{

class DiffCommand: public ICommand
{
public:
  DiffCommand() {}
  void runAlgorithm() override;
};

}
