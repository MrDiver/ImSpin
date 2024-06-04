#pragma once
#include "config.hpp"
#include <TextEditor.h>
#include <filesystem>
#include <map>
#include <mutex>
#include <string>

class TempDir
{
  public:
    TempDir(std::filesystem::path path);
    ~TempDir();

  private:
    std::filesystem::path previous;
};

class SpinWrap
{
  public:
    SpinWrap();
    ~SpinWrap();
    // Config Functions
    void readConfigFile();
    void writeConfigFile();
    void makePathsAbsolute();
    bool isConfigValid();
    Config &mutConfig();
    Config const &getConfig();
    SimulationSettings &mutSettings();
    SimulationSettings const &getSettings();
    static bool readFile(fs::path path, std::string &filecontent);
    // Editor
    std::map<int, std::string> generateErrorMarkers(std::string code);

  private:
    SimulationSettings settings;
    Config config;
    // Common
    std::filesystem::path tmppath;
};
