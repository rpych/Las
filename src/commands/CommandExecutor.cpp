#include "CommandExecutor.hpp"

namespace las::commands
{

CommandExecutor::CommandExecutor()
{}

void CommandExecutor::execute()
{
  command->runAlgorithm();
}

void CommandExecutor::setCommand(std::unique_ptr<las::ui::ICmdParser> parser)
{
  command = createCommand(std::move(parser));
}

std::unique_ptr<ICommand> CommandExecutor::createCommand(std::unique_ptr<las::ui::ICmdParser> parser)
{
  auto mode{getExecutionMode(parser->getOptions())};
  auto filenames {std::move(getFilenames(parser->getFilenames()))};
  std::unique_ptr<common::AFileParser> fileParser{std::make_unique<common::FileParser>()};
  auto params {CommandParams{.fileParser=std::move(fileParser),
                             .mode=mode,
                             .filenames=filenames}};
  switch (parser->getCommand())
  {
    case common::LasCmd::CUT:
      return std::make_unique<las::commands::CutCommand>(params);
      break;
    case common::LasCmd::DIFF:
      return std::make_unique<las::commands::DiffCommand>(params);
      break;
    case common::LasCmd::DIFF_HEAD:
      return std::make_unique<las::commands::DiffHeadCommand>(params);
      break;
    case common::LasCmd::DIFF_STAGED:
      return std::make_unique<las::commands::DiffStagedCommand>(params);
      break;
    case common::LasCmd::RESTORE:
      return std::make_unique<las::commands::RestoreCommand>(parser->getOptions());
      break;
    default:
      return nullptr;
  }
}

common::LasCmdOpts CommandExecutor::getExecutionMode(las::ui::LasParsedOptions const& opts)
{
  auto foundOption = std::find_if(opts.options.begin(), opts.options.end(), [](auto const option)
  {
    return (option == common::LasCmdOpts::SINGLE_MODE or option == common::LasCmdOpts::PARALLEL_MODE);
  });
  return (foundOption != opts.options.end() ? *foundOption : common::LasCmdOpts::INVALID);
}

std::optional<FilenamesVector> CommandExecutor::getFilenames(FilenamesVector const& filenames)
{
  return (filenames.size() > 0 ? std::optional<FilenamesVector>{filenames}
                               : std::optional<FilenamesVector>{});
}

}
