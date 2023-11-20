#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include "../commands/common/Utils.hpp"

namespace las::ui
{

namespace common = las::commands::common;

class ICmdParser
{
public:
  ICmdParser() {};
  virtual void parse(char const* args[]) = 0;
  virtual ~ICmdParser() = default;
};

class CmdParser: public ICmdParser
{
public:
  CmdParser(int _argc): argc(_argc) {}
  void parse(char const* args[]) override;
private:
  bool recognizeCmdPart(std::string_view cmdPart);
  void addFilename(std::string_view filename);

  int argc{};
  std::vector<std::string> inputFilenames{};
  common::LasCmd command{common::LasCmd::INVALID};
  std::set<common::LasCmdOpts> options{};
};

}
