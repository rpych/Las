#pragma once
#include "ICommand.hpp" 

namespace las::commands
{

class CutCommand: public ICommand
{
public:
  void runAlgorithm();
};

}