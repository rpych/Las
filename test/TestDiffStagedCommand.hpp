#pragma once
#include "../src/commands/ICommand.hpp"
#include "../src/commands/ACommandWrapper.hpp"
#include "testcases/Utils.hpp"

namespace las::test
{
using namespace commands;
class TestDiffStagedCommand: public ICommand, public ACommandWrapper
{
public:
  TestDiffStagedCommand(CommandParams& params): ACommandWrapper(params) {}
  std::string GIT_CHECKOUT_STASH_WORK_AREA{"git checkout stash@{1}"};
  void runAlgorithm() override
  {
    std::cout<<"runAlgorithm for TEST  DIFF staged command"<<std::endl;
    auto const allFilenames(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_HEAD_FILES)));
    if (allFilenames.size() == 0)
    {
      return;
    }
    auto const filenamesWorkArea(std::move(getAllFilenames(common::GitCmd::GIT_DIFF_FILES)));
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_PUSH_KEEP_INDEX);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_APPLY);

    auto const& filenames = (cmdLineFilenames) ? getFilteredFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES)
                                               : getAllFilenames(common::GitCmd::GIT_DIFF_STAGED_FILES);
    fileParser->parse(filenames);
    fileWriter->write(fileParser->getFilesHunks());

    for (auto const& f: filenamesWorkArea)
    {
      std::string cmdForFile{std::format("{} {}", GIT_CHECKOUT_STASH_WORK_AREA, f)};
      if (std::find(filenames.begin(), filenames.end(), f) == filenames.end())
      {
        executeOsCommandWithoutResult(cmdForFile);
      }
    }
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP_1);
    osCommandProxy->executeOsCommandNotSave(common::GitCmd::GIT_STASH_DROP);
  }
};

}