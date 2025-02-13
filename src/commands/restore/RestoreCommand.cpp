#include "RestoreCommand.hpp"

namespace las::commands
{

RestoreCommand::RestoreCommand(las::ui::LasParsedOptions const lasOptions): lasOptions(lasOptions)
{}

std::string RestoreCommand::lasBackupDir = "";
std::string RestoreCommand::cmdStagedAreaBackup = "";
std::string RestoreCommand::cmdWorkAreaBackup = "";

void RestoreCommand::runAlgorithm()
{
  logLasDebug("RestoreCommand::runAlgorithm");
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
    common::executeCommand(std::format("less -R {}/{}", RestoreCommand::lasBackupDir, chosenFilename).c_str());
  }
  else if (lasOptions.isNthElemOptionValid() and lasOptions.options.size() == 0)
  {
    std::string const& filenamesBundle{osCommandProxy->executeOsCommand(common::GitCmd::GIT_DIFF_HEAD_FILES)};
    auto const notSavedFilenames{std::move(common::getFilenamesFromStatusCmd(filenamesBundle))};
    if (notSavedFilenames.size() > 0)
    {
      //error-don't run las restore on unsaved changes
      logLasError("Save current changes before running las restore command!");
      return;
    }
    logLasDebug("RestoreCommand::runAlgorithm apply restored patch");
    auto const& chosenFilename{backupFilenames.at(lasOptions.nthElemOption.value())};
    common::executeCommand(std::format("git apply --reject {}/{}", RestoreCommand::lasBackupDir, chosenFilename).c_str());
  }
  else
  {
    logLasError("No valid options for las restore command provided");
  }
}

void RestoreCommand::saveCurrentState()
{
  if (not common::rdRestoreEnabled) { return; }

  logLasDebug("RestoreCommand::saveCurrentState");
  std::vector<std::string> backupFilenames{std::move(getListedFiles())};
  if (backupFilenames.size() >= BackupMaxEntries)
  {
    std::sort(backupFilenames.begin(), backupFilenames.end());
    std::string oldestBackupFilename{backupFilenames.front()};
    logLasDebug("RestoreCommand::saveCurrentState file to remove: {}/{}", RestoreCommand::lasBackupDir, oldestBackupFilename);
    common::executeCommand(std::format("rm {}/{}", RestoreCommand::lasBackupDir, oldestBackupFilename).c_str());
  }

  std::string cmdResult{};
  common::saveCommandResult("git diff HEAD", cmdResult);
  std::stringstream contentStream{cmdResult};

  auto const currentTime{std::chrono::system_clock::now()};
  auto const newBackupFile{std::format("{:%Y-%m-%d_%H:%M:%OS}.diff", currentTime)};
  logLasDebug("RestoreCommand::saveCurrentState new backup file: {}", newBackupFile);
  common::writeContentToFile(contentStream, std::format("{}/{}", RestoreCommand::lasBackupDir, newBackupFile));
}

void RestoreCommand::setLasBackupDir()
{
  std::string cmdResult{};
  common::saveCommandResult("git rev-parse --show-toplevel", cmdResult);
  auto const rootRepoDir(cmdResult.substr(0, cmdResult.length()-1));
  RestoreCommand::lasBackupDir = std::format("{}/{}", rootRepoDir, LasStateBackupDir);
  RestoreCommand::cmdStagedAreaBackup = std::format("{}/{}", rootRepoDir, StagedAreaFilesBackup);
  RestoreCommand::cmdWorkAreaBackup = std::format("{}/{}", rootRepoDir, WorkAreaFilesBackup);
}

std::vector<std::string> RestoreCommand::getListedFiles()
{
  std::string cmdResult{};
  common::saveCommandResult(std::format("ls -1 {}", lasBackupDir).c_str(), cmdResult);
  return common::getFilenamesFromStatusCmd(cmdResult);
}

}