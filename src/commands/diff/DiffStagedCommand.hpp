#pragma once
#include "ICommand.hpp"
#include "ACommandWrapper.hpp"
#include "Utils.hpp"
#include "../common/parsers/FileParser.hpp"

namespace las::commands
{

class DiffStagedCommand: public ICommand, public ACommandWrapper
{
public:
  DiffStagedCommand();
  void runAlgorithm() override;
};

}
