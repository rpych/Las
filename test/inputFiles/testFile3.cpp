//^^las begin^^
#include <string>
#include <iostream>
//^^las end^^
//^^las sub begin^^
//#include <string>
//#include <iostream>//needed for printing
//^^las sub end^^


struct Car
{
  std::string model;//^^las^^  std::string model{};
  int year{};

  bool isTooOldToDriveIntoKrakow(int yearThresh)
  {
    //^^las begin^^
    if(year < yearThresh)
    {
      std::cout<<"isTooOldToDriveIntoKrakow year:"<<year<<", yearThresh:"<<yearThresh<<std::endl;
      return true;
    }
    //^^las end^^
    return false;
  }

};

//^^las begin^^
int w{28};
//^^las end^^
//^^las sub begin^^
//int w = 28;
//^^las sub end^^
