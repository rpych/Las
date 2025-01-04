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

  virtual FilenamesVector const getAllFilenames()
  {
    auto const filenamesBundle = osCommandProxy->executeOsCommand(common::GitCmd::GIT_DIFF_HEAD_FILES);
    return common::getFilenamesFromStatusCmd(filenamesBundle);
  }

  std::unique_ptr<common::AOSCommandProxy<GET_VCS>> osCommandProxy;
  std::unique_ptr<common::AFileParser> fileParser;
  std::unique_ptr<common::AFileWriter> fileWriter;
  std::optional<FilenamesVector> cmdLineFilenames;
};


}