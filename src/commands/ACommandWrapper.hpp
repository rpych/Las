#pragma once
#include "common/OSCommandProxy.hpp"
#include "common/FileDiffParser.hpp"


namespace las::commands
{

class ACommandWrapper
{
public:
  virtual ~ACommandWrapper() = default;

protected:
  ACommandWrapper(): osCommandProxy{std::make_unique<common::OSCommandProxy<common::GitCmd>>()},
                     fileDiffParser{std::make_unique<common::FileDiffParser>()} {}

  std::unique_ptr<common::AOSCommandProxy<common::GitCmd>> osCommandProxy;
  std::unique_ptr<common::AFileDiffParser> fileDiffParser;
};


}