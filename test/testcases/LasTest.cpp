#include "LasTest.hpp"

namespace las::test
{

void LasTest::checkLasSystemBehaviour()
{
  for (int i=0; i<testFilenames.size(); ++i)
  {
    auto const testFilePath{"tmp/inputFiles/"+testFilenames.at(i)};
    std::stringstream testFileContentStream{};
    readTestFileContent(testFileContentStream, testFilePath);
    auto const modelTestFilePath{"modelFiles/"+modelTestFilenames.at(i)};
    std::cout<<"Test Files: testFilePath:"<<testFilePath<<", modelTestFilePath:"<<modelTestFilePath<<std::endl;
    std::stringstream modelTestFileContentStream{};
    readTestFileContent(modelTestFileContentStream, modelTestFilePath);
    compareFilesContent(testFileContentStream, modelTestFileContentStream);
  }
}

void LasTest::compareFilesContent(std::stringstream& testContent, std::stringstream& modelContent)
{
  std::string lineTest{}, lineModel{};
  while(std::getline(testContent, lineTest) and std::getline(modelContent, lineModel))
  {
    if (lineTest != lineModel)
    {
      std::cout<<"RPYCH::ERROR: lineTest:"<<lineTest<<", lineModel:"<<lineModel<<std::endl;
    }
    //EXPECT_STREQ(lineTest.c_str(), lineModel.c_str());
    lineTest.clear();
    lineModel.clear();
  }
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
  
  lasApp->run(args);
  checkLasSystemBehaviour();
  EXPECT_TRUE(false);
}


//LasTestParam lasTestParams[1] = {LasTestParam{2, "las diff", {TEST_FILE1_MODEL}}};

INSTANTIATE_TEST_SUITE_P(LasTestAllVariants, LasTest,
                         Values(LasTestParam{2, "las diff", {TEST_FILE1_MODEL}, {}}));

}