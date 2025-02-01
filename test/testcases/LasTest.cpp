#include "LasTest.hpp"

namespace las::test
{


void LasTestBase::checkLasSystemBehaviour(std::vector<std::string> const& filenamesToCheck,
                                      std::vector<std::string> const& modelFilenames)
{
  for (int i=0; i<filenamesToCheck.size(); ++i)
  {
    auto const testFilePath{std::format("{}/{}", TEST_EXEC_ROOT_DIR, filenamesToCheck.at(i))};
    std::stringstream testFileContentStream{};
    readTestFileContent(testFileContentStream, testFilePath);
    auto const modelTestFilePath{std::format("{}/{}", TEST_ROOT_DIR, modelFilenames.at(i))};
    std::stringstream modelTestFileContentStream{};
    readTestFileContent(modelTestFileContentStream, modelTestFilePath);
    bool result {compareFilesContent(testFileContentStream, modelTestFileContentStream)};
    std::cout<<"Test Files: testFilePath:"<<testFilePath<<", modelTestFilePath:"<<modelTestFilePath<<", result:"<<result<<std::endl;
  }
}

bool LasTestBase::compareFilesContent(std::stringstream& testContent, std::stringstream& modelContent)
{
  std::cout<<"compareFilesContent"<<std::endl;
  std::string lineTest{}, lineModel{};
  bool result{true};
  int lineNum{0};
  while(std::getline(testContent, lineTest) and std::getline(modelContent, lineModel))
  {
    if (lineTest != lineModel)
    {
      result = false;
      std::cout<<"ERROR: lineTest:"<<lineTest<<", lineModel:"<<lineModel<<std::endl;
    }
    EXPECT_STREQ(lineTest.c_str(), lineModel.c_str());
    lineTest.clear();
    lineModel.clear();
    lineNum++;
  }
  std::cout<<"compareFilesContent for "<<lineNum<<" lines"<<std::endl;
  return result;
}

TEST_P(LasTest, CheckLasHunksHandling)
{
  commands::common::rdRestoreEnabled = false;
  auto const& param = GetParam();
  for (auto const& p : param.setupCommands)
  {
    executeOsCommandWithoutResult(p);
  }
  
  std::vector<std::string> const args {std::move(prepareArgsForLasSystem(param.inputCmd))};
  
  lasApp->run(args);
  LasTestBase::checkLasSystemBehaviour(param.filenamesToCheck, param.modelFilenames);
}

TEST_P(LasTestWholeApp, CheckLasCommands)
{
  commands::common::rdRestoreEnabled = false;
  auto const& param = GetParam();
  for (auto const& p : param.setupCommands)
  {
    executeOsCommandWithoutResult(p);
  }
  
  std::vector<std::string> const args {std::move(prepareArgsForLasSystem(param.inputCmd))};
  auto const filesStagedAreaBefore{std::move(getAllTestFilenames("git diff --staged --name-only"))};
  auto const filesWorkAreaBefore{std::move(getAllTestFilenames("git diff --name-only"))};

  lasApp->run(args);
  LasTestBase::checkLasSystemBehaviour(param.filenamesToCheck, param.modelFilenames);
  auto const filesStagedAreaAfter{std::move(getAllTestFilenames("git diff --staged --name-only"))};
  auto const filesWorkAreaAfter{std::move(getAllTestFilenames("git diff --name-only"))};
  bool resStagedArea{compareFilesInAreas(filesStagedAreaBefore, filesStagedAreaAfter)};
  bool resWorkArea{compareFilesInAreas(filesWorkAreaBefore, filesWorkAreaAfter)};
  EXPECT_TRUE(resStagedArea);
  EXPECT_TRUE(resWorkArea);
  std::string result{};
  executeOsCommandWithResult("git status tmp/", result);
  std::cout<<"Git status at the end\n"<<result<<std::endl;
}

TEST_P(LasTestRestoreCommand, CheckLasRestoreCommand)
{
  commands::common::rdRestoreEnabled = true;
  auto const& param = GetParam();
  for (auto const& p : param.setupCommands)
  {
    executeOsCommandWithoutResult(p);
  }

  for (auto& [cmd, argc] : param.prerequisiteCmds)
  {
    lasApp = std::make_unique<Las>(argc);
    std::vector<std::string> const args {std::move(prepareArgsForLasSystem(cmd))};
    lasApp->run(args);
    lasApp.reset();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  executeOsCommandWithoutResult(std::format("git restore --staged --worktree {}", TEST_EXEC_ROOT_DIR));
  std::vector<std::string> const mainCmdArgs {std::move(prepareArgsForLasSystem(param.inputCmd))};

  lasApp = std::make_unique<Las>(param.argc);
  lasApp->run(mainCmdArgs);
  LasTestBase::checkLasSystemBehaviour(param.filenamesToCheck, param.modelFilenames);
  std::string result{};
  executeOsCommandWithResult("git status tmp/", result);
  std::cout<<"Git status at restore end\n"<<result<<std::endl;
}

INSTANTIATE_TEST_SUITE_P(LasTestAllVariantsCPP, LasTest, Values(
  LasTestParam{.argc=2,
               .inputCmd="las diff",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={}},
  LasTestParam{.argc=2,
               .inputCmd="las diff",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)}},
  LasTestParam{.argc=3,
               .inputCmd="las diff HEAD",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)}},
  LasTestParam{.argc=3,
               .inputCmd="las diff staged",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)}},

  LasTestParam{.argc=3,
               .inputCmd="las diff staged",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={}},
  LasTestParam{.argc=2,
               .inputCmd="las cut",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)}},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las diff", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                  joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={}},
  LasTestParam{.argc=5,
               .inputCmd=joinCmdPaths("las diff head", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                                                       joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)}},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las diff staged", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP), ""),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)}},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las diff", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                  joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_MODEL_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP, MODEL_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP, TEST_FILE2_MODEL_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP, TEST_FILE2_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)}},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las cut", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                 joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)}},
   LasTestParam{.argc=5, /*test invalid case*/
               .inputCmd=joinCmdPaths("las diff head", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                       joinPath(TEST_EXEC_ROOT_DIR, INVALID_TEST_FILE_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)}},
  LasTestParam{.argc=5, /*test with patch application*/
               .inputCmd=joinCmdPaths("las diff head", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                       joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_PATCH_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                               joinCmd("git apply", TEST_ROOT_DIR, PATCH_TEST_FILE2_CPP) }},
LasTestParam{.argc=5, /*test with patch application*/
              .inputCmd=joinCmdPaths("las diff staged", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP),
                                                        joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)),
              .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
              .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
              .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_MODEL_CPP},
              .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP),
                              joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                              joinCmd("git apply", TEST_ROOT_DIR, PATCH_TEST_FILE2_CPP) }}



));

//-----------------------------------

INSTANTIATE_TEST_SUITE_P(LasTestWholeAppCPP, LasTestWholeApp, Values(
  LasTestParam{.argc=2,
               .inputCmd="las diff",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=2,
               .inputCmd="las diff",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=3,
               .inputCmd="las diff HEAD",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=3,
               .inputCmd="las diff staged",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)},
               .isTestCmdExecutor=false},

  LasTestParam{.argc=3,
               .inputCmd="las diff staged",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=2,
               .inputCmd="las cut",
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_MODEL_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las diff", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                  joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=5,
               .inputCmd=joinCmdPaths("las diff head", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                                                       joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las diff staged", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP), ""),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las diff", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                  joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_MODEL_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP, MODEL_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP, TEST_FILE2_MODEL_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP, TEST_FILE2_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=4,
               .inputCmd=joinCmdPaths("las cut", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                 joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_CPP, TEST_FILE3_MODEL_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)},
               .isTestCmdExecutor=false},
   LasTestParam{.argc=5, /*test invalid case*/
               .inputCmd=joinCmdPaths("las diff head", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                       joinPath(TEST_EXEC_ROOT_DIR, INVALID_TEST_FILE_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)},
               .isTestCmdExecutor=false},
  LasTestParam{.argc=5, /*test with patch application*/
               .inputCmd=joinCmdPaths("las diff head", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                                                       joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)),
               .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
               .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
               .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_PATCH_CPP, TEST_FILE3_CPP},
               .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP),
                               joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                               joinCmd("git apply", TEST_ROOT_DIR, PATCH_TEST_FILE2_CPP) },
               .isTestCmdExecutor=false},
  LasTestParam{.argc=5, /*test with patch application*/
                .inputCmd=joinCmdPaths("las diff staged", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP),
                                                          joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP)),
                .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
                .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
                .modelFilenames={TEST_FILE1_CPP, TEST_FILE2_PATCH_CPP, TEST_FILE3_CPP},
                .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE3_CPP),
                                joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE2_CPP),
                                joinCmd("git apply", TEST_ROOT_DIR, PATCH_TEST_FILE2_CPP) },
                .isTestCmdExecutor=false}
               
               
));

//----------------------------------------------------------

INSTANTIATE_TEST_SUITE_P(LasTestRestoreCmdCPP, LasTestRestoreCommand, Values(
  LasTestParamManyCmds{.argc=3,
                       .inputCmd="las restore -n0",
                       .prerequisiteCmds={{joinCmdPaths("las cut", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP), ""), 3},
                                          {joinCmdPaths("las diff HEAD", joinPath(TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP), ""), 4}},
                       .inputDirsToCopy={INPUT_TEST_FILES_DIR_CPP},
                       .filenamesToCheck={TEST_FILE1_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
                       .modelFilenames={TEST_FILE1_MODEL_CPP, TEST_FILE2_CPP, TEST_FILE3_CPP},
                       .setupCommands={joinCmd("git add", TEST_EXEC_ROOT_DIR, TEST_FILE1_CPP)}}


));


}