#include <iostream>
#include "commands/CutCommand.hpp"

namespace commands = las::commands;

int main()
{
  commands::CutCommand cutCommand{};
  std::cout<<"Hello from LAS (Log, Analyze and Share system) app"<<std::endl;
  return 0;
}