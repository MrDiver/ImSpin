#pragma once
#include <map>
#include <string>

namespace spin
{
std::map<int, std::string> generateErrorMarkers(std::string code);
std::string formatFile(std::string code);
} // namespace spin
