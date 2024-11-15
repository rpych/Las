#pragma once
#include <string>
#include <sstream>

namespace las::commands::common
{

void saveCommandResult(char const* cmd, std::string& result);
void readFileContent(std::stringstream& contentStream, std::string const& filename);
void writeContentToFile(std::stringstream& contentStream, std::string const& filename);

}
