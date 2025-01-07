#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <variant>
#include "../commands/common/Utils.hpp"

namespace las::ui
{

namespace common = las::commands::common;
class CmdParser;

//commands
struct Cut{};
struct Diff{};
struct Head{};
struct Staged{};
//options
struct Filename{ std::string name{}; };
struct ParallelMode{};
//states
struct StateParseCommand{};
struct StateCommandKnown{};
struct StateParseFilenames{};
struct StateParseOtherOptions{};
struct StateInvalid{};

class ICmdParser
{
public:
  ICmdParser() {};
  virtual void parse(char const* args[]) = 0;
  virtual std::vector<std::string> const& getFilenames() = 0;
  virtual common::LasCmd const getCommand() = 0;
  virtual std::set<common::LasCmdOpts> const& getOptions() = 0;
  virtual ~ICmdParser() = default;
};

class CmdParser: public ICmdParser
{
public:
  using CmdState = std::variant<StateParseCommand, StateCommandKnown, StateParseFilenames, StateParseOtherOptions, StateInvalid>;
  using CmdEvent = std::variant<Cut, Diff, Head, Staged, Filename, ParallelMode>;

  struct CmdParserSm
  {
    CmdParserSm(CmdParser& cmdParser) : parser(cmdParser) {}

    CmdState operator()(StateParseCommand&, Diff&)
    {
      parser.command = common::LasCmd::DIFF;
      return StateCommandKnown{};
    }
    CmdState operator()(StateParseCommand&, Cut&)
    {
      parser.command = common::LasCmd::CUT;
      return StateCommandKnown{};
    }
    CmdState operator()(StateCommandKnown&, Head&)
    {
      if (parser.command == common::LasCmd::DIFF) { parser.command = common::LasCmd::DIFF_HEAD; }
      else { std::cout<<"Error.\"HEAD\" used with wrong command name!"<<std::endl; }
      return StateParseFilenames{};
    }
    CmdState operator()(StateCommandKnown&, Staged&)
    {
      if (parser.command == common::LasCmd::DIFF) { parser.command = common::LasCmd::DIFF_STAGED; }
      else { std::cout<<"Error.\"staged\" used with wrong command name!"<<std::endl; }
      return StateParseFilenames{};
    }
    CmdState operator()(StateCommandKnown&, ParallelMode&)
    {
      parser.options.erase(common::LasCmdOpts::SINGLE_MODE);
      parser.options.emplace(common::LasCmdOpts::PARALLEL_MODE);
      return StateParseOtherOptions{};
    }
    CmdState operator()(StateCommandKnown&, Filename& file)
    {
      parser.addFilename(file.name);
      return StateParseFilenames{};
    }
    CmdState operator()(StateParseFilenames&, Filename& file)
    {
      parser.addFilename(file.name);
      return StateParseFilenames{};
    }
    CmdState operator()(StateParseFilenames&, ParallelMode&)
    {
      parser.options.erase(common::LasCmdOpts::SINGLE_MODE);
      parser.options.emplace(common::LasCmdOpts::PARALLEL_MODE);
      return StateParseOtherOptions{};
    }
    CmdState operator()(auto&, auto&)
    {
      std::cout<<"Error.Wrong las command name or option used!"<<std::endl;
      return StateInvalid{};
    }

    CmdParser& parser;
  };

  CmdParser(int _argc): argc(_argc) {}
  void parse(char const* args[]) override;
  std::vector<std::string> const& getFilenames() override;
  common::LasCmd const getCommand() override;
  std::set<common::LasCmdOpts> const& getOptions() override;
private:
  void recognizeCmdPart(std::string_view cmdPart);
  void addFilename(std::string_view filename);

  int argc{};
  std::vector<std::string> inputFilenames{};
  common::LasCmd command{common::LasCmd::INVALID};
  std::set<common::LasCmdOpts> options{common::LasCmdOpts::SINGLE_MODE};
  CmdState state{StateParseCommand{}};
  CmdEvent event{};
};

}
