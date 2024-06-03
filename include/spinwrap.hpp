#pragma once
#include <TextEditor.h>
#include <map>
#include <string>

namespace spin
{
std::map<int, std::string> generateErrorMarkers(std::string code);
std::string formatFile(std::string code);
void generateSymbolTable();
void runProgram();
} // namespace spin

TextEditor::LanguageDefinition createPromelaLanguage();
