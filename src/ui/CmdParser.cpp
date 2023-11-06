#include "CmdParser.hpp"
#include <algorithm>
#include <iterator>

namespace las::ui
{

void CmdParser::parse(char const* args[])
{
  for(int i=1; i < argc; ++i)
  {
    std::string_view lexem{args[i]};
    std::string lowercasedWord{};
    std::transform(std::begin(lexem), std::end(lexem), std::back_inserter(lowercasedWord),
                                      [](auto const c){ return std::tolower(c); });
    if (auto res = recognizeCmdPart(lowercasedWord)) continue;
    addFilename(args[i]);
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

bool CmdParser::recognizeCmdPart(std::string_view cmdPart)
{
  bool res{};
  if (command == common::LasCmd::INVALID and cmdPart == "diff")
  {
    command = common::LasCmd::DIFF;
    res = true;
  }
  else if(command != common::LasCmd::INVALID and cmdPart == "--include")
  {
    options.insert(common::LasCmdOpts::INCLUDE);
    res = true;
  }
  return res;
}

void CmdParser::addFilename(std::string_view filename)
{
  inputFilenames.push_back(static_cast<std::string>(filename));
}

}