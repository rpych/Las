#pragma once
#include "../ICommand.hpp"
#include "../ACommandWrapper.hpp"

namespace las::commands
{

class DiffStagedCommand: public ICommand, public ACommandWrapper
{
public:
  DiffStagedCommand(CommandParams& params);
  void runAlgorithm() override;
};

}
