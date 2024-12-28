#pragma once
#include "ICommand.hpp"
#include "ACommandWrapper.hpp"
#include "Utils.hpp"
#include "../common/parsers/FileParser.hpp"

namespace las::commands
{

class DiffHeadCommand: public ICommand, public ACommandWrapper
{
public:
  DiffHeadCommand();
  void runAlgorithm() override;
};

}
