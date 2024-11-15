#include <algorithm>
#include "FileWriter.hpp"
#include "ACodeWriter.hpp"
#include "CodeWriterExclude.hpp"

namespace las::commands::common
{

void FileWriter::write(std::unordered_map<std::string, std::vector<LasHunk>>const& filesHunks)
{
  std::unique_ptr<ACodeWriter> codeWriter{createCodeWriter(option)};
  std::for_each(filesHunks.begin(), filesHunks.end(), [&codeWriter](auto const& hunkInfo)
  {
    auto const& [filename, hunk] = hunkInfo;
    std::cout<<"Filename with hunks:"<<filename<<std::endl;
    codeWriter->write(filename, hunk);
  });
}

std::unique_ptr<ACodeWriter> FileWriter::createCodeWriter(LasCmdOpts option)
{
  switch (option)
  {
    case LasCmdOpts::EXCLUDE:
      return std::make_unique<CodeWriterExclude>();
    default:
      return nullptr;
  }
}

}