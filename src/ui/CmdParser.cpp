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

std::set<common::LasCmdOpts> const& CmdParser::getOptions()
{
  return options;
}

void CmdParser::recognizeCmdPart(std::string_view cmdPart)
{
  if (cmdPart == "diff")
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
  else
  {
    event = Filename{.name=std::string(cmdPart)};
    state = std::visit(CmdParserSm{*this}, state, event);
  }
  std::cout<<"cmdPart:"<<cmdPart<<", event:"<< event.index()<<std::endl;
}

void CmdParser::addFilename(std::string_view filename)
{
  inputFilenames.push_back(static_cast<std::string>(filename));
}

}
