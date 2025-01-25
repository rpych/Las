#pragma once
#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <format>

#include "Utils.hpp"
#include "../TestCommandExecutor.hpp"
#include "../../src/executors/Las.hpp"

namespace las::test
{

using ::testing::TestWithParam;
using ::testing::Values;

struct LasTestParam
{
  int argc{};
  std::string inputCmd;
  std::vector<std::string> filenamesToCheck{};
  std::vector<std::string> setupCommands{};
};

class LasTest : public TestWithParam<LasTestParam>
{
public:
  using Las = executors::Las; 
  LasTest() = default;
  
  void SetUp() override
  {
    auto const param = GetParam();
    lasApp = std::make_unique<Las>(param.argc,
                                   std::make_unique<TestCommandExecutor>());

    std::string result1{};
    executeOsCommandWithResult("pwd", result1);
    std::cout<<"RPY::from CheckLasHunksHandling test, result:"<<result1<<std::endl;
    executeOsCommandWithoutResult(std::format("rm -rf {}", TEST_EXEC_ROOT_DIR));
    //executeOsCommandWithoutResult(std::format("mkdir {}", TEST_EXEC_ROOT_DIR));
    executeOsCommandWithoutResult(std::format("mkdir -p {}/{}", TEST_EXEC_ROOT_DIR, INPUT_TEST_FILES_DIR));
    executeOsCommandWithoutResult(std::format("git add {}/{}", TEST_EXEC_ROOT_DIR, INPUT_TEST_FILES_DIR));
    for (auto const& f: testFilenames)
    {
      auto const filenamePath{std::format("{}/{}", INPUT_TEST_FILES_DIR, f)};
      auto const createFileCommand{std::format("touch {}/{}", TEST_EXEC_ROOT_DIR, filenamePath)};
      executeOsCommandWithoutResult(createFileCommand);
      auto const gitAddFileCommand{std::format("git add {}/{}", TEST_EXEC_ROOT_DIR, filenamePath)};
      executeOsCommandWithoutResult(gitAddFileCommand);
      std::stringstream contentStream{};
      auto const destFilenamePath{std::format("{}/{}", TEST_EXEC_ROOT_DIR, filenamePath)};
      auto const sourceFilenamePath{std::format("{}/{}", TEST_ROOT_DIR, filenamePath)};
      readTestFileContent(contentStream, sourceFilenamePath);
      writeTestFileContent(contentStream, destFilenamePath);
    }
    std::string result{};
    executeOsCommandWithResult("git status -v tmp/inputFiles/", result);
  }

  void TearDown() override
  {
    lasApp.reset();

    //executeOsCommandWithoutResult(std::format("rm -rf {}", TEST_EXEC_ROOT_DIR));
  }

protected:
  void checkLasSystemBehaviour(std::vector<std::string> const& filenamesToCheck);
  bool compareFilesContent(std::stringstream& testContent, std::stringstream& modelContent);

  std::vector<std::string> testFilenames{TEST_FILE1, TEST_FILE2, TEST_FILE3};
  std::vector<std::string> modelTestFilenames{TEST_FILE1_MODEL, TEST_FILE2_MODEL, TEST_FILE3_MODEL};
  std::unique_ptr<Las> lasApp;
};

}