#pragma once
#include <map>
#include <string>
#include <TextEditor.h>

namespace spin
{
std::map<int, std::string> generateErrorMarkers(std::string code);
std::string formatFile(std::string code);
} // namespace spin


TextEditor::LanguageDefinition createPromelaLanguage();