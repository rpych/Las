#pragma once
#include "ICommand.hpp"
#include "ACommandWrapper.hpp"

namespace las::commands
{

class DiffHeadCommand: public ICommand, public ACommandWrapper
{
public:
  DiffHeadCommand(CommandParams& params);
  void runAlgorithm() override;
};

}
