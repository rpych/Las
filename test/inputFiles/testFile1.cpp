#include "testFile2.hpp" //^^las^^
#include <utility>
#include <array>//^^las^^#include <vector>

#include <string>
//^^las begin^^
#include <iostream>
//this is only needed to check las system behaviour
//^^las end^^
//^^las sub begin^^
//#include <array>
//#include <iostream>
//^^las sub end^^

namespace school
{

class Student
{
public:
  Student() = default; ////^^las^^  Student() {}
  Student(std::string n, int a): name(n), age(a) {}
private:
  std::string name{};
  int age{};
  std::array<std::string, 5> subjects{"math", "history", "science", "economy", "polish"};
  

  std::string const& getName()
  {
    return name;
  }

  //^^las begin^^  
  int getAge()
  {
    return age;
  }
 //^^las end^^
/*^^las sub begin^^
  int getAge()
  {
    std::cout<<"Age:"<<age<<std::endl;
    return age;

  }
  ^^las sub end^^ */

  void printFunny(int size, std::string s1, std::string s2, std::string s3, std::string s4, std::string s5)
  {
    printf("I would like to print the following content: subjectsNum:%d, 1:%s, 2:%s, 3:%s, 4:%s, 5:%s",
    size, s1.c_str(), s2.c_str(), s3.c_str(), s4.c_str(), s5.c_str());
  }

  void print()
  {
    printFunny(
    5,//^^las^^
    //^^las sub begin^^
//    subjects.size(),
    //^^las sub end^^
    subjects[0],
    subjects[1],
    subjects[2],
    subjects[3],   //^^las^^    "economy",
    subjects[4]
    );
  }

};



}

