#include <iostream>
#include <string>
#include "executors/Las.hpp"

namespace executors = las::executors;

int main(int argc, const char* argv[])
{
  std::cout<<"Hello from LAS (Log, Analyze and Share system) app"<<std::endl;
  std::vector<std::string> args;
  for (int i=0; i<argc; ++i)
  {
    args.push_back(argv[i]);
  }
  executors::Las las(argc);
  las.run(args);
  return 0;
}
