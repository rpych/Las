#include "Las.hpp"

namespace las::executors
{

Las::Las(int argc):
  parser(std::make_unique<las::ui::CmdParser>(argc)),
  cmdExecutor(std::make_unique<las::commands::CommandExecutor>())                 
{}

void Las::run(char const* args[])
{
  parser->parse(args);
  //find command to execute from parsed output
  cmdExecutor->setCommand(std::make_unique<las::commands::DiffCommand>());
  cmdExecutor->execute();
}

}
