#pragma once
#include "../ICommand.hpp"
#include "../ACommandWrapper.hpp"

namespace las::commands
{

class CutCommand: public ICommand, public ACommandWrapper
{
public:
  CutCommand(CommandParams& params);
  void runAlgorithm() override;
};

}
