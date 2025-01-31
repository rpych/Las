#include "CmdParser.hpp"
#include <algorithm>
#include <iterator>

namespace las::ui
{

void CmdParser::parse(std::vector<std::string> const& args)
{
  for(int i=1; i < argc; ++i)
  {
    std::string_view lexem{args.at(i)};
    std::string lowercasedWord{};
    std::transform(std::begin(lexem), std::end(lexem), std::back_inserter(lowercasedWord),
                                      [](auto const c){ return std::tolower(c); });
    recognizeCmdPart(lowercasedWord);
  }
  //temp line below
  std::copy(inputFilenames.begin(), inputFilenames.end(), std::ostream_iterator<std::string>(std::cout, ", "));
  if (inputFilenames.size() == 0)
  {
    //process all touched files
    std::cout<<"\nProcess all touched files"<<std::endl;
  }
  else
  {
    //process chosen files
    std::cout<<"\nProcess only chosen files size"<<inputFilenames.size()<<std::endl;
  }
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
    auto const n{std::stoi(static_cast<std::string>(cmdPart.substr(2)))};
    event = NthElement{n};
    state = std::visit(CmdParserSm{*this}, state, event);
    return true;
  }
  return false;
}

void CmdParser::recognizeCmdPart(std::string_view cmdPart)
{
  if (recognizeRestoreCmdPart(cmdPart))
  { std::cout<<"Restore command part parsed"<<std::endl;}
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
  else if (common::fileExists(static_cast<std::string>(cmdPart)))
  {
    event = Filename{.name=std::string(cmdPart)};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  else
  {
    std::cout<<"Unrecognized option or not existing file: "<<cmdPart<<std::endl;
    return;
  }
  std::cout<<"cmdPart:"<<cmdPart<<", event:"<< event.index()<<std::endl;
}

void CmdParser::addFilename(std::string_view filename)
{
  inputFilenames.push_back(static_cast<std::string>(filename));
}

}
