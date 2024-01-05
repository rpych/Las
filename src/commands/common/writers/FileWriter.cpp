#include "FileWriter.hpp"
#include "ACodeWriter.hpp"
#include "CodeWriterExclude.hpp"
#include "CodeWriterInclude.hpp"
#include "../parsers/FileParser.hpp"

namespace las::commands::common
{

void FileWriter::write(std::unique_ptr<AFileParser> fileParser)
{
  std::unique_ptr<ACodeWriter> codeWriter{createCodeWriter(option)};
  codeWriter->write();
}

std::unique_ptr<ACodeWriter> FileWriter::createCodeWriter(LasCmdOpts option)
{
  switch (option)
  {
    case LasCmdOpts::EXCLUDE:
      return std::make_unique<CodeWriterExclude>();
    case LasCmdOpts::INCLUDE:
      return std::make_unique<CodeWriterInclude>();
    default:
      return nullptr;
  }
}

}