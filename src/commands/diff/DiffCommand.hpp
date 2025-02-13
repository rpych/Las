#pragma once
#include "../ICommand.hpp"
#include "../ACommandWrapper.hpp"
#include "../restore/RestoreCommand.hpp"

namespace las::commands
{
class DiffCommand: public ICommand, public ACommandWrapper
{
public:
  DiffCommand(CommandParams& params);
  void runAlgorithm() override;
};

}
