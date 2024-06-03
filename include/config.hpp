#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <filesystem>

enum SimulationMode
{
    RANDOM = 0,
    INTERACTIVE,
    GUIDED
};

enum ChannelMode
{
    BLOCKING = 0,
    LOSING,
};

struct Config
{
    std::vector<std::string> recentFiles;
    float global_scale = 0.3;
    float syntaxCheckDelay = 0.5;
    #ifdef PC
    std::string spin_path = "extra/spin.exe";
    std::string swarm_path = "extra/swarm.exe";
    std::string vcvarsall_path = "";
    #else
    std::string spin_path = "extra/spin";
    std::string swarm_path = "extra/swarm";
    #endif
    std::string dot_path = "dot";
    std::string compiler_path = "";
    // Simulation Settings
    // # Mode
    int seed = 123;
    SimulationMode simulationMode = RANDOM;
    char trailPath[256] = "";
    int initialStepsSkipped = 0;
    int maximumNumberOfSteps = 10000;
    bool trackDataValues = false;
    // # Channel
    ChannelMode channelMode = BLOCKING;
    bool MSCStmnt = false;
    int MSCMaxTextWidth = 20;
    int MSCUpdateDelay = 25;
    // # Filter
    char filterProcessIds[256] = "";
    char filterQueueIds[256] = "";
    char filterVarNames[256] = "";
    char filterTrackedVariable[256] = "";
    char filterTrackScaling[256] = "";
    // Common
    std::filesystem::path tmppath;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, recentFiles, global_scale, syntaxCheckDelay, spin_path, swarm_path, dot_path, compiler_path);

extern Config config;
