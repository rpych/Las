#pragma once
#include "common/OSCommandProxy.hpp"
#include "common/parsers/FileParser.hpp"
#include <memory>


namespace las::commands
{

class ACommandWrapper
{
public:
  virtual ~ACommandWrapper() = default;

protected:
  ACommandWrapper(): osCommandProxy{std::make_unique<common::OSCommandProxy<common::GitCmd>>()},
                     fileParser{std::make_unique<common::FileParser>()} {}

  std::unique_ptr<common::AOSCommandProxy<common::GitCmd>> osCommandProxy;
  std::unique_ptr<common::AFileParser> fileParser;
};


}