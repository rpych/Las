#pragma once
#include <sstream>
#include <iterator>
#include "../../src/commands/common/OSProxyUtils.hpp"
#include "../../src/commands/common/Utils.hpp"

namespace las::test
{
//CPP
const std::string INPUT_TEST_FILES_DIR_CPP{"inputFilesCPP"};
const std::string MODEL_TEST_FILES_DIR_CPP{"modelFilesCPP"};
//Elixir
const std::string INPUT_TEST_FILES_DIR_ELIXIR{"inputFilesElixir"};
const std::string MODEL_TEST_FILES_DIR_ELIXIR{"modelFilesElixir"};
//Python
const std::string INPUT_TEST_FILES_DIR_PYTHON{"inputFilesPython"};
const std::string MODEL_TEST_FILES_DIR_PYTHON{"modelFilesPython"};
//CPP
const std::string TEST_FILE1_CPP{"inputFilesCPP/testFile1.cpp"};
const std::string TEST_FILE2_CPP{"inputFilesCPP/testFile2.hpp"};
const std::string TEST_FILE3_CPP{"inputFilesCPP/testFile3.cpp"};
const std::string TEST_FILE2_PATCH_CPP{"inputFilesCPP/testFile2Patch.hpp"};
const std::string INVALID_TEST_FILE_CPP{"inputFilesCPP/testFile10.cpp"};
const std::string PATCH_TEST_FILE2_CPP{"inputFilesCPP/patchTestFile2.diff"};
const std::string TEST_FILE1_MODEL_CPP{"modelFilesCPP/testFile1Model.cpp"};
const std::string TEST_FILE2_MODEL_CPP{"modelFilesCPP/testFile2Model.hpp"};
const std::string TEST_FILE3_MODEL_CPP{"modelFilesCPP/testFile3Model.cpp"};
const std::string TEST_FILE2_MODEL_PATCH_CPP{"modelFilesCPP/testFile2ModelPatch.hpp"};
//Elixir
const std::string TEST_FILE1_ELIXIR{"inputFilesElixir/testFile1.exs"};
const std::string TEST_FILE1_MODEL_ELIXIR{"modelFilesElixir/testFile1Model.exs"};
//Python
const std::string TEST_FILE1_PYTHON{"inputFilesPython/testFile1.py"};
const std::string TEST_FILE1_MODEL_PYTHON{"modelFilesPython/testFile1Model.py"};

const std::string TEST_ROOT_DIR{"test"};
const std::string TEST_EXEC_ROOT_DIR{"tmp"};



inline void executeOsCommandWithoutResult(std::string const& cmd)
{
  las::commands::common::executeCommand(cmd.c_str());
}

inline void executeOsCommandWithResult(std::string const& cmd, std::string& result)
{
  las::commands::common::saveCommandResult(cmd.c_str(), result);
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

std::vector<std::string> const getAllTestFilenames(std::string const& cmd)
{
  std::string result{};
  executeOsCommandWithResult(cmd.c_str(), result);
  return las::commands::common::getFilenamesFromStatusCmd(result);
}

bool compareFilesInAreas(std::vector<std::string> const& before,
                          std::vector<std::string> const& after)
{
  bool res = std::all_of(before.begin(), before.end(), [&after](auto const& f)
  {
    return (std::find(after.begin(), after.end(), f) != after.end());
  });
  return res;
}

inline std::string joinCmd(std::string const& cmd, std::string const& prefix, std::string const& file)
{
  return std::format("{} {}/{}", cmd, prefix, file);
}

inline std::string joinPath(std::string const& prefix, std::string const& file)
{
  return std::format("{}/{}", prefix, file);
}

inline std::string joinCmdPaths(std::string const& cmd, std::string const& file1, std::string const& file2)
{
  return std::format("{} {} {}", cmd, file1, file2);
}

}