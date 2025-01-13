#pragma once
#include <gtest/gtest.h>
#include <vector>
#include <memory>

#include "Utils.hpp"
#include "../../src/executors/Las.hpp"

namespace las::test
{

using ::testing::TestWithParam;
using ::testing::Values;

struct LasTestParam
{
  int argc{};
  std::string inputCmd;
  std::vector<std::string> modelFilenames{};
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
    lasApp = std::make_unique<Las>(param.argc);

    std::string result1{};
    executeOsCommandWithResult("pwd", result1);
    std::cout<<"RPY::from CheckLasHunksHandling test, result:"<<result1<<std::endl;
    executeOsCommandWithoutResult("rm -rf tmp");
    executeOsCommandWithoutResult("mkdir tmp");
    executeOsCommandWithoutResult("mkdir -p tmp/inputFiles");
    executeOsCommandWithoutResult("git add -f tmp/inputFiles");
    for (auto const& f: testFilenames)
    {
      auto const filenamePath{"inputFiles/"+f};
      auto const createFileCommand{"touch tmp/"+filenamePath};
      executeOsCommandWithoutResult(createFileCommand.c_str());
      auto const gitAddFileCommand{"git add tmp/"+filenamePath};
      executeOsCommandWithoutResult(gitAddFileCommand.c_str());
      std::stringstream contentStream{};
      auto const destFilenamePath{"tmp/"+filenamePath};
      readTestFileContent(contentStream, filenamePath);
      writeTestFileContent(contentStream, destFilenamePath);
    }
    std::string result{};
    executeOsCommandWithResult("git status -v tmp/inputFiles/", result);
  }

  void TearDown() override
  {
    lasApp.reset();

    //executeOsCommandWithoutResult("rm -rf inputFiles/ ");
  }

protected:
  void checkLasSystemBehaviour();
  void compareFilesContent(std::stringstream& testContent, std::stringstream& modelContent);

  std::vector<std::string> testFilenames{TEST_FILE1, TEST_FILE2, TEST_FILE3};
  std::vector<std::string> modelTestFilenames{TEST_FILE1_MODEL, TEST_FILE2_MODEL, TEST_FILE3_MODEL};
  std::unique_ptr<Las> lasApp;
};

}