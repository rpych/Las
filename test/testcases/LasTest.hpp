#pragma once
#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <format>
#include <thread>
#include <chrono>

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
  std::string inputCmd{};
  std::vector<std::string> inputDirsToCopy{};
  std::vector<std::string> filenamesToCheck{};
  std::vector<std::string> modelFilenames{};
  std::vector<std::string> setupCommands{};
  bool isTestCmdExecutor{true};
};

struct LasTestParamManyCmds
{
  using CmdCombined = std::pair<std::string, int>;
  int argc{};
  std::string inputCmd{};
  std::vector<CmdCombined> prerequisiteCmds{};
  std::vector<std::string> inputDirsToCopy{};
  std::vector<std::string> filenamesToCheck{};
  std::vector<std::string> modelFilenames{};
  std::vector<std::string> setupCommands{};
};

class LasTestBase
{
public:
  using Las = executors::Las;
  LasTestBase() = default;
  void setup(std::vector<std::string> const& inputDirsToCopy,
             std::vector<std::string> const& filenamesToCheck)
  {
    executeOsCommandWithoutResult(std::format("rm -rf {}", TEST_EXEC_ROOT_DIR));
    for (auto const& dir: inputDirsToCopy)
    {
      executeOsCommandWithoutResult(std::format("mkdir -p {}/{}", TEST_EXEC_ROOT_DIR, dir));
      executeOsCommandWithoutResult(std::format("git add {}/{}", TEST_EXEC_ROOT_DIR, dir));
    }

    for (auto const& f: filenamesToCheck)
    {
      auto const createFileCommand{std::format("touch {}/{}", TEST_EXEC_ROOT_DIR, f)};
      executeOsCommandWithoutResult(createFileCommand);
      auto const gitAddFileCommand{std::format("git add {}/{}", TEST_EXEC_ROOT_DIR, f)};
      executeOsCommandWithoutResult(gitAddFileCommand);
      std::stringstream contentStream{};
      auto const destFilenamePath{std::format("{}/{}", TEST_EXEC_ROOT_DIR, f)};
      auto const sourceFilenamePath{std::format("{}/{}", TEST_ROOT_DIR, f)};
      readTestFileContent(contentStream, sourceFilenamePath);
      writeTestFileContent(contentStream, destFilenamePath);
    }
  }

  void teardown()
  {
    lasApp.reset();
    executeOsCommandWithoutResult(std::format("git restore --staged {}", TEST_EXEC_ROOT_DIR));
  }

protected:
  void checkLasSystemBehaviour(std::vector<std::string> const& filenamesToCheck, std::vector<std::string> const& modelFilenames);
  bool compareFilesContent(std::stringstream& testContent, std::stringstream& modelContent);

  std::unique_ptr<Las> lasApp{};
};


class LasTest : public LasTestBase,
                public TestWithParam<LasTestParam>
{
public:
  LasTest() = default;

  void SetUp() override
  {
    auto const param = GetParam();
    lasApp = std::make_unique<Las>(param.argc,
                                   std::make_unique<TestCommandExecutor>());
    LasTestBase::setup(param.inputDirsToCopy, param.filenamesToCheck);
  }

  void TearDown() override
  {
    std::cout<<"TearDown invoked"<<std::endl;
    LasTestBase::teardown();
  }
};


class LasTestWholeApp : public LasTestBase,
                        public TestWithParam<LasTestParam>
{
public:
  LasTestWholeApp() = default;
  void SetUp() override
  {
    auto const param = GetParam();
    lasApp = std::make_unique<Las>(param.argc);
    LasTestBase::setup(param.inputDirsToCopy, param.filenamesToCheck);
  }

  void TearDown() override
  {
    LasTestBase::teardown();
  }
};

class LasTestRestoreCommand : public LasTestBase,
                              public TestWithParam<LasTestParamManyCmds>
{
public:
  LasTestRestoreCommand() = default;
  void SetUp() override
  {
    auto const param = GetParam();
    LasTestBase::setup(param.inputDirsToCopy, param.filenamesToCheck);
  }

  void TearDown() override
  {
    LasTestBase::teardown();
  }
};

}
