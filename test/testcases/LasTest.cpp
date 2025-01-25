#include "LasTest.hpp"

namespace las::test
{

void LasTest::checkLasSystemBehaviour(std::vector<std::string> const& filenamesToCheck)
{
  for (int i=0; i<testFilenames.size(); ++i)
  {
    if (std::find(filenamesToCheck.begin(), filenamesToCheck.end(), testFilenames.at(i)) != filenamesToCheck.end())
    {
      auto const testFilePath{std::format("{}/{}/{}", TEST_EXEC_ROOT_DIR, INPUT_TEST_FILES_DIR, testFilenames.at(i))};
      std::stringstream testFileContentStream{};
      readTestFileContent(testFileContentStream, testFilePath);
      auto const modelTestFilePath{std::format("{}/{}", MODEL_TEST_FILES_DIR, modelTestFilenames.at(i))};
      std::stringstream modelTestFileContentStream{};
      readTestFileContent(modelTestFileContentStream, modelTestFilePath);
      bool result {compareFilesContent(testFileContentStream, modelTestFileContentStream)};
      std::cout<<"Test Files: testFilePath:"<<testFilePath<<", modelTestFilePath:"<<modelTestFilePath<<", result:"<<result<<std::endl;
    }
  }
}

bool LasTest::compareFilesContent(std::stringstream& testContent, std::stringstream& modelContent)
{
  std::string lineTest{}, lineModel{};
  bool result{true};
  while(std::getline(testContent, lineTest) and std::getline(modelContent, lineModel))
  {
    if (lineTest != lineModel)
    {
      result = false;
      std::cout<<"RPYCH::ERROR: lineTest:"<<lineTest<<", lineModel:"<<lineModel<<std::endl;
    }
    //EXPECT_STREQ(lineTest.c_str(), lineModel.c_str());
    lineTest.clear();
    lineModel.clear();
  }
  return result;
}

TEST_P(LasTest, CheckLasHunksHandling)
{
  auto const& param = GetParam();
  std::string result{};
  for (auto const& p : param.setupCommands)
  {
    executeOsCommandWithoutResult(p.c_str());
  }
  
  executeOsCommandWithResult("git diff tmp/inputFiles/", result);
  std::vector<std::string> const args {std::move(prepareArgsForLasSystem(param.inputCmd))};
  
  // lasApp = std::make_unique<Las>(param.argc);
  //lasApp->cmdExecutor = std::make_unique<TestCommandExecutor>();
  lasApp->run(args);
  checkLasSystemBehaviour(param.filenamesToCheck);
  EXPECT_TRUE(false);
}


//LasTestParam lasTestParams[1] = {LasTestParam{2, "las diff", {TEST_FILE1_MODEL}}};
//LasTestParam{2, "las diff HEAD", {TEST_FILE2}, {}}
INSTANTIATE_TEST_SUITE_P(LasTestAllVariants, LasTest,
                         Values(LasTestParam{2, "las diff", {TEST_FILE2}, {}}
                                ));
 
}