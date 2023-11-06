#include "Las.hpp"

namespace las::executors
{

Las::Las(int argc): parser(std::make_unique<las::ui::CmdParser>(argc))
{}

void Las::run(char const* args[])
{
  parser->parse(args);
}

}