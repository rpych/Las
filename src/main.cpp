#include <iostream>
#include <string>
#include "executors/Las.hpp"

namespace executors = las::executors;

int main(int argc, const char* argv[])
{
  std::cout<<"Hello from LAS (Log, Analyze and Share system) app"<<std::endl;
  executors::Las las(argc);
  las.run(argv);
  return 0;
}