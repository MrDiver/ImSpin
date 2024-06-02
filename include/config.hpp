#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct Config
{
    std::vector<std::string> recentFiles;
    float global_scale = 0.3;
    float syntaxCheckDelay = 0.5;
    std::string spin_path = "extra/spin";
    std::string swarm_path = "extra/swarm";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, recentFiles, global_scale);

extern Config config;
