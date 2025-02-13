#include "CmdParser.hpp"
#include <algorithm>
#include <iterator>

namespace las::ui
{

bool CmdParser::parse(std::vector<std::string> const& args)
{
  bool parsingResult{true};
  for(int i=1; i < argc; ++i)
  {
    parsingResult = parsingResult and recognizeCmdPart(args.at(i));
  }
  return parsingResult;
}

std::vector<std::string> const& CmdParser::getFilenames()
{
  return inputFilenames;
}

common::LasCmd const CmdParser::getCommand()
{
  return command;
}

LasParsedOptions const& CmdParser::getOptions()
{
  return lasOptions;
}

bool CmdParser::recognizeRestoreCmdPart(std::string_view cmdPart)
{
  if (cmdPart == "restore")
  {
    event = Restore{};
    state = std::visit(CmdParserSm{*this}, state, event);
    return true;
  }
  else if (cmdPart == "--list")
  {
    event = List{};
    state = std::visit(CmdParserSm{*this}, state, event);
    return true;
  }
  else if (cmdPart == "--show")
  {
    event = Show{};
    state = std::visit(CmdParserSm{*this}, state, event);
    return true;
  }
  else if (cmdPart.substr(0, 2) == "-n")
  {
    auto lexem{cmdPart.substr(2)};
    if (not common::isReprNumber(lexem)) { return false; }
    auto const n{std::stoi(static_cast<std::string>(lexem))};
    event = NthElement{n};
    state = std::visit(CmdParserSm{*this}, state, event);
    return true;
  }
  return false;
}

bool CmdParser::recognizeCmdPart(std::string_view rawCmdPart)
{
  bool result{true};
  std::string cmdPart{};
  std::transform(std::begin(rawCmdPart), std::end(rawCmdPart), std::back_inserter(cmdPart),
                                    [](auto const c){ return std::tolower(c); });

  if (recognizeRestoreCmdPart(cmdPart))
  {
    logLasDebug("Restore command part parsed");
  }
  else if (cmdPart == "diff")
  {
    event = Diff{};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else if (cmdPart == "cut")
  {
    event = Cut{};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else if (cmdPart == "head")
  {
    event = Head{};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else if (cmdPart == "staged")
  {
    event = Staged{};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else if (cmdPart == "--par")
  {
    event = ParallelMode{};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else if (common::fileExists(static_cast<std::string>(rawCmdPart)))
  {
    event = Filename{.name=std::string(rawCmdPart)};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else
  {
    logLasError("Unrecognized option or not existing file: {}.\n See las --help", rawCmdPart);
    result = false;
  }

  if (std::holds_alternative<StateInvalid>(state))
  {
    result = false;
  }
  logLasDebug("cmdPart:{}, event:{}", rawCmdPart, event.index())
  return result;
}

void CmdParser::addFilename(std::string_view filename)
{
  inputFilenames.push_back(static_cast<std::string>(filename));
}

}
