#pragma once
#include <sstream>
#include <iterator>
#include "../../src/commands/common/OSProxyUtils.hpp"

namespace las::test
{

const std::string TEST_FILE1{"testFile1.cpp"};
const std::string TEST_FILE2{"testFile2.hpp"};
const std::string TEST_FILE3{"testFile3.cpp"};
const std::string TEST_FILE1_MODEL{"testFile1Model.cpp"};
const std::string TEST_FILE2_MODEL{"testFile2Model.hpp"};
const std::string TEST_FILE3_MODEL{"testFile3Model.cpp"};
//const std::string INPUT_FILES_DIR{"inputFiles"};
//const std::string MODEL_FILES_DIR{"modelFiles"};


inline void executeOsCommandWithoutResult(char const* cmd)
{
  las::commands::common::executeCommand(cmd);
}

inline void executeOsCommandWithResult(char const* cmd, std::string& result)
{
  las::commands::common::saveCommandResult(cmd, result);
}

inline void writeTestFileContent(std::stringstream& contentStream, std::string const& filename)
{
  las::commands::common::writeContentToFile(contentStream, filename);
}

inline void readTestFileContent(std::stringstream& contentStream, std::string const& filename)
{
  las::commands::common::readFileContent(contentStream, filename);
}

inline std::vector<std::string> const prepareArgsForLasSystem(std::string const& command)
{
  std::stringstream cmdStream{command};
  std::vector<std::string> args{};
  std::string word{};
  while(std::getline(cmdStream, word, ' '))
  {
    args.push_back(word);
  }
  return args;
}

}