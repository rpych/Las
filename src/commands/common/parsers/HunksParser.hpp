#pragma once
#include <cstdint>
#include <stack>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <string>
#include "../../../Const.hpp"
#include "../Utils.hpp"
#include "LasParserSmManager.hpp"

namespace las::commands::common
{

class HunksParser
{
public:
  HunksParser(std::shared_ptr<LasLanguage> lasLang) : lpSm(lasLang) {}
  bool parseForHunks(std::stringstream& s);
  std::vector<LasHunk> getHunks() const { return lpSm.hunks; };

private:
  LasParserSmManager lpSm;
};

}
