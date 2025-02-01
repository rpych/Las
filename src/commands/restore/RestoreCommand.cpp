#include "RestoreCommand.hpp"

namespace las::commands
{

RestoreCommand::RestoreCommand(las::ui::LasParsedOptions const lasOptions): lasOptions(lasOptions)
{}

std::string RestoreCommand::lasBackupDir = "";

void RestoreCommand::runAlgorithm()
{
  std::cout<<"runAlgorithm for RESTORE command"<<std::endl;
  std::vector<std::string> backupFilenames{getListedFiles()};
  std::sort(backupFilenames.begin(), backupFilenames.end(), std::greater<std::string>());
  if (lasOptions.contains(common::LasCmdOpts::LIST))
  {
    for (int i=0; i<backupFilenames.size(); ++i)
    {
      std::cout<<std::format("n{}--{}", i, backupFilenames.at(i))<<std::endl;
    }
  }
  else if(lasOptions.contains(common::LasCmdOpts::SHOW) and lasOptions.isNthElemOptionValid())
  {
    auto const& chosenFilename{backupFilenames.at(lasOptions.nthElemOption.value())};
    common::executeCommand(std::format("cat {}/{} | less", RestoreCommand::lasBackupDir, chosenFilename).c_str());
  }
  else if (lasOptions.isNthElemOptionValid() and lasOptions.options.size() == 0)
  {
    std::string const& filenamesBundle{osCommandProxy->executeOsCommand(common::GitCmd::GIT_DIFF_HEAD_FILES)};
    auto const notSavedFilenames{std::move(common::getFilenamesFromStatusCmd(filenamesBundle))};
    if (notSavedFilenames.size() > 0)
    {
      //error-don't run las restore on unsaved changes
      std::cout<<"Save current changes before running las restore command!"<<std::endl;
      return;
    }
    std::cout<<"Apply restored patch"<<std::endl;
    auto const& chosenFilename{backupFilenames.at(lasOptions.nthElemOption.value())};
    common::executeCommand(std::format("git apply --reject {}/{}", RestoreCommand::lasBackupDir, chosenFilename).c_str());
    //common::executeCommand(std::format("git apply --check {}/{}", RestoreCommand::lasBackupDir, chosenFilename).c_str());
  }
  else
  {
    std::cout<<"No valid options for las restore command provided"<<std::endl;
  }
}

void RestoreCommand::saveCurrentState()
{
  if (not common::rdRestoreEnabled) { return; }

  std::cout<<"saveCurrentState"<<std::endl;
  // cmdResult.clear();
  // common::saveCommandResult(std::format("ls -1 {}", lasBackupDir).c_str(), cmdResult);
  // std::cout<<"Files from Restore lasBackupDir:"<<lasBackupDir<<std::endl;
  std::vector<std::string> backupFilenames{std::move(getListedFiles())};
  if (backupFilenames.size() >= BackupMaxEntries)
  {
    std::sort(backupFilenames.begin(), backupFilenames.end());
    std::string oldestBackupFilename{backupFilenames.front()};
    std::cout<<"File to remove oldestBackupFilename:"<<std::format("{}/{}", RestoreCommand::lasBackupDir, oldestBackupFilename)<<std::endl;
    common::executeCommand(std::format("rm {}/{}", RestoreCommand::lasBackupDir, oldestBackupFilename).c_str());
  }
  //common::executeCommand(std::format("touch {}/{}", RestoreCommand::lasBackupDir, newBackupFile).c_str());

  std::string cmdResult{};
  common::saveCommandResult("git diff HEAD", cmdResult);
  std::stringstream contentStream{cmdResult};

  auto const currentTime{std::chrono::system_clock::now()};
  auto const newBackupFile{std::format("{:%Y-%m-%d_%H:%M:%OS}.diff", currentTime)};
  std::cout<<"from Restore newBackupFile:"<<newBackupFile<<",lasBackupDir:"<<RestoreCommand::lasBackupDir<<std::endl;

  std::cout<<"Time file to write diff:"<<std::format("{}/{}", RestoreCommand::lasBackupDir, newBackupFile)<<std::endl;
  common::writeContentToFile(contentStream, std::format("{}/{}", RestoreCommand::lasBackupDir, newBackupFile));
}

void RestoreCommand::setLasBackupDir()
{
  std::string cmdResult{};
  common::saveCommandResult("git rev-parse --show-toplevel", cmdResult);
  auto const rootRepoDir(cmdResult.substr(0, cmdResult.length()-1));
  RestoreCommand::lasBackupDir = std::format("{}/{}", rootRepoDir, LasHiddenDirectory);
}

std::vector<std::string> RestoreCommand::getListedFiles()
{
  std::string cmdResult{};
  common::saveCommandResult(std::format("ls -1 {}", lasBackupDir).c_str(), cmdResult);
  std::cout<<"Files from Restore::getListedFiles lasBackupDir:"<<lasBackupDir<<std::endl;
  return common::getFilenamesFromStatusCmd(cmdResult);
}

}