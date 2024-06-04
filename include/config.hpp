#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;

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
    int theme              = 0;
    float global_scale     = 0.2;
    float syntaxCheckDelay = 0.3;
    fs::path spin_path     = "extra/spin";
    fs::path swarm_path    = "extra/swarm";
    fs::path dot_path      = "extra/dot";
    fs::path compiler_path = "gcc";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, theme, global_scale, syntaxCheckDelay, spin_path, swarm_path, dot_path,
                                   compiler_path);

struct SimulationSettings
{
    // # Mode
    bool trackDataValues          = false;
    int seed                      = 123;
    int initialStepsSkipped       = 0;
    int maximumNumberOfSteps      = 10000;
    SimulationMode simulationMode = RANDOM;
    fs::path trailPath            = "";
    // # Channel
    bool MSCStmnt           = false;
    int MSCMaxTextWidth     = 20;
    int MSCUpdateDelay      = 25;
    ChannelMode channelMode = BLOCKING;
    // # Filter
    std::string filterProcessIds      = "";
    std::string filterQueueIds        = "";
    std::string filterVarNames        = "";
    std::string filterTrackedVariable = "";
    std::string filterTrackScaling    = "";
};
