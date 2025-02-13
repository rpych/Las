#pragma once
#include <chrono>
#include <format>
#include <algorithm>
#include <functional>
#include "../ICommand.hpp"
#include "../ACommandWrapper.hpp"
#include "../common/OSProxyUtils.hpp"
#include "../common/Utils.hpp"
#include "../common/OSCommandProxy.hpp"
#include "../../Const.hpp"
#include "../../ui/CmdParser.hpp"

namespace las::commands
{

class RestoreCommand: public ICommand
{
public:
  using AOSCommandProxy = common::AOSCommandProxy<GET_VCS>;
  using OSCommandProxy  = common::OSCommandProxy<GET_VCS>;
  RestoreCommand(las::ui::LasParsedOptions const lasOptions);
  void runAlgorithm() override;
  static void saveCurrentState();
  static void setLasBackupDir();
  static std::string lasBackupDir;
  static std::string cmdStagedAreaBackup;
  static std::string cmdWorkAreaBackup;
private:
  static std::vector<std::string> getListedFiles();

  static inline std::unique_ptr<AOSCommandProxy> osCommandProxy{std::make_unique<OSCommandProxy>()};
  las::ui::LasParsedOptions const lasOptions;
};

}  