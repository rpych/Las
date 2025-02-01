#include <string>
#include <iostream>//needed for printing


struct Car
{
  std::string model{};
  int year{};

  bool isTooOldToDriveIntoKrakow(int yearThresh)
  {
    return false;
  }

};

int w = 28;