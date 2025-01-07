#pragma once
#include <vector>
#include <algorithm>
#include <optional>
#include "../Utils.hpp"
//#include "../parsers/HunksParser.hpp"

namespace las::commands::common
{

inline std::optional<LasHunk> getLasHunkContainingLine(std::vector<LasHunk> const& lasHunks, uint64_t lineNum)
{
  auto res = std::find_if(lasHunks.begin(), lasHunks.end(), [lineNum](auto const& hunk)
  {
    return hunk.contains(lineNum);
  });
  return (res != lasHunks.end() ? *res : std::optional<LasHunk>(std::nullopt));
}
}

class Baba; //^^las^^ class BaBaDaDa;

struct Raptor{}; //^^las^^  
//^^las sub begin^^

//trala lala
//^^las sub end^^

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
