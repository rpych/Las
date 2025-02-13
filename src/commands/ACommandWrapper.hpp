#pragma once
#include "common/OSCommandProxy.hpp"
#include "common/parsers/FileParser.hpp"
#include "common/writers/FileWriter.hpp"
#include <memory>


namespace las::commands
{

using FilenamesVector = std::vector<std::string>;
struct CommandParams
{
  std::unique_ptr<common::AFileParser> fileParser;
  common::LasCmdOpts mode{common::LasCmdOpts::SINGLE_MODE}; //PARALLEL_MODE
  std::optional<FilenamesVector> filenames{};
};

class ACommandWrapper
{
public:
  virtual ~ACommandWrapper() = default;

protected:
  ACommandWrapper(CommandParams& params) :
                     osCommandProxy{std::make_unique<common::OSCommandProxy<GET_VCS>>()},
                     fileParser(std::move(params.fileParser)),
                     fileWriter{std::make_unique<common::FileWriter>(params.mode)},
                     cmdLineFilenames(params.filenames){}

  FilenamesVector const getAllFilenames(common::GitCmd cmd)
  {
    auto const filenamesBundle = osCommandProxy->executeOsCommand(cmd);
    return common::getFilenamesFromStatusCmd(filenamesBundle);
  }

  FilenamesVector const getFilteredFilenames(common::GitCmd cmd)
  {
    auto const filenamesBundle = osCommandProxy->executeOsCommand(cmd);
    auto const availFilenames{std::move(common::getFilenamesFromStatusCmd(filenamesBundle))};
    std::vector<std::string> filtered;
    if (cmdLineFilenames)
    {
      auto const cmdPresentFilenames{*cmdLineFilenames};
      std::copy_if(cmdPresentFilenames.begin(), cmdPresentFilenames.end(), std::back_inserter(filtered),
      [&availFilenames](std::string const& f)
      {
        auto res = std::find(availFilenames.begin(), availFilenames.end(), f) != availFilenames.end();
        return res;
      });
    }
    return filtered;
  }

  std::unique_ptr<common::AOSCommandProxy<GET_VCS>> osCommandProxy;
  std::unique_ptr<common::AFileParser> fileParser;
  std::unique_ptr<common::AFileWriter> fileWriter;
  std::optional<FilenamesVector> cmdLineFilenames;
};


}