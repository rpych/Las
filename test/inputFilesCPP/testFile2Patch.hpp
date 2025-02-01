#pragma once
#include <iostream>

class SingleClass; //^^las^^ class SingleHunkClass;

struct SubstitutionStruct{}; //^^las^^  
//^^las sub begin^^

double value{}; 
////this is comment example
//^^las sub end^^


int getMax(int a, int b)
{
  if (a > b)
  {
    return a;
  }
  return b;
}

struct Las
{
  std::string name{"Las"};
  int year{2025};
  void toString()
  {
    /*^^las begin^^*/
  //this is not needed comment
  /*^^las end^^*/
    std::cout<<name<<"::"<<year<<std::endl;
  }

};


//^^las begin^^
int fun()
{
  double c = 0.2;
  return c;
}

//^^las end^^
//^^las sub begin^^
void substFun()
{
  std::cout<<"Function comes from substitution LasHunk"<<std::endl;
}
//^^las sub end^^

