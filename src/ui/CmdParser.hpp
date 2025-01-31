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
struct Restore{};
struct Head{};
struct Staged{};
//options
struct Filename{ std::string name{}; };
struct ParallelMode{};
struct List{};
struct NthElement{ int n{}; };
struct Show{};
//states
struct StateParseCommand{};
struct StateCommandKnown{};
struct StateRestoreCommandKnown{};
struct StateParseFilenames{};
struct StateParseOtherOptions{};
struct StateEnd{};
struct StateInvalid{};

struct LasParsedOptions
{
  std::set<common::LasCmdOpts> options{common::LasCmdOpts::SINGLE_MODE};
  std::optional<int> nthElemOption{}; //for -n option

  bool contains(common::LasCmdOpts opt) const
  {
    return options.count(opt) > 0;
  }

  bool isNthElemOptionValid() const
  {
    return nthElemOption.has_value() and *(nthElemOption) < BackupMaxEntries;
  }
};

class ICmdParser
{
public:
  ICmdParser() {};
  virtual void parse(std::vector<std::string> const& args) = 0;
  virtual std::vector<std::string> const& getFilenames() = 0;
  virtual common::LasCmd const getCommand() = 0;
  virtual LasParsedOptions const& getOptions() = 0;
  virtual ~ICmdParser() = default;
};

class CmdParser: public ICmdParser
{
public:
  using CmdState = std::variant<StateParseCommand, StateCommandKnown, StateRestoreCommandKnown, StateParseFilenames, StateParseOtherOptions, StateInvalid>;
  using CmdEvent = std::variant<Cut, Diff, Restore, Head, Staged, Filename, ParallelMode, List, NthElement, Show>;

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
    CmdState operator()(StateParseCommand&, Restore&)
    {
      parser.command = common::LasCmd::RESTORE;
      return StateRestoreCommandKnown{};
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
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.options.emplace(common::LasCmdOpts::PARALLEL_MODE);
      return StateParseOtherOptions{};
    }
    CmdState operator()(StateCommandKnown&, Filename& file)
    {
      parser.addFilename(file.name);
      return StateParseFilenames{};
    }
    CmdState operator()(StateRestoreCommandKnown&, List&)
    {
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.options.emplace(common::LasCmdOpts::LIST);
      return StateParseOtherOptions{};
    }
    CmdState operator()(StateRestoreCommandKnown&, Show&)
    {
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.options.emplace(common::LasCmdOpts::SHOW);
      return StateParseOtherOptions{};
    }
    CmdState operator()(StateRestoreCommandKnown&, NthElement& elem)
    {
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.nthElemOption = std::make_optional<>(elem.n);
      return StateParseCommand{};
    }
    CmdState operator()(StateParseFilenames&, Filename& file)
    {
      parser.addFilename(file.name);
      return StateParseFilenames{};
    }
    CmdState operator()(StateParseFilenames&, ParallelMode&)
    {
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.options.emplace(common::LasCmdOpts::PARALLEL_MODE);
      return StateParseOtherOptions{};
    }
    CmdState operator()(StateParseOtherOptions&, Filename& file)
    {
      //return invalid if restore command
      parser.addFilename(file.name);
      return StateParseFilenames{};
    }
    CmdState operator()(StateParseOtherOptions&, ParallelMode&)
    {
      //return invalid if restore command
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.options.emplace(common::LasCmdOpts::PARALLEL_MODE);
      return StateParseOtherOptions{};
    }
    CmdState operator()(StateParseOtherOptions&, NthElement& elem)
    {
      //return invalid if not restore command 
      auto& lo{parser.lasOptions};
      lo.options.erase(common::LasCmdOpts::SINGLE_MODE);
      lo.nthElemOption = std::make_optional<>(elem.n);
      return StateParseCommand{};
    }
    CmdState operator()(auto&, auto&)
    {
      std::cout<<"Error.Wrong las command name or option used!"<<std::endl;
      return StateInvalid{};
    }

    CmdParser& parser;
  };

  CmdParser(int _argc): argc(_argc) {}
  void parse(std::vector<std::string> const& args) override;
  std::vector<std::string> const& getFilenames() override;
  common::LasCmd const getCommand() override;
  LasParsedOptions const& getOptions() override;
private:
  void recognizeCmdPart(std::string_view cmdPart);
  bool recognizeRestoreCmdPart(std::string_view cmdPart);
  void addFilename(std::string_view filename);

  int argc{};
  std::vector<std::string> inputFilenames{};
  common::LasCmd command{common::LasCmd::INVALID};
  LasParsedOptions lasOptions{};
  CmdState state{StateParseCommand{}};
  CmdEvent event{};
};

}
