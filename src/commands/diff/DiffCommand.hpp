#pragma once
#include "ICommand.hpp"
#include "ACommandWrapper.hpp"
#include "Utils.hpp"
#include "../common/parsers/FileParser.hpp"

namespace las::commands
{

class DiffCommand: public ICommand, public ACommandWrapper
{
public:
  DiffCommand();
  void runAlgorithm() override;
private:
  std::string const& getOsCommandResult(common::GitCmd cmd); //maybe redundant
};

}
