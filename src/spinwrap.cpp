#include "spinwrap.hpp"
#include <boost/process.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;
namespace bp = boost::process;
namespace xp = boost::xpressive;

TempDir::TempDir(fs::path path)
{
    this->previous = fs::absolute(fs::current_path());
    fs::current_path(path);
}

TempDir::~TempDir()
{
    fs::current_path(previous);
}

SpinWrap::SpinWrap() : tmppath(fs::temp_directory_path())
{
}

SpinWrap::~SpinWrap()
{
}

void SpinWrap::readConfigFile()
{
    try
    {
        std::ifstream file("config.json");
        if (file)
        {
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string config_content = buffer.str();
            nlohmann::json j           = nlohmann::json::parse(config_content);
            config                     = j.template get<Config>();
        }
    }
    catch (std::exception e)
    {
        spdlog::warn("Failed to load config.json, most likely you updated ImSpin and the definition changed");
    }
}

void SpinWrap::writeConfigFile()
{
    spdlog::info("Writing Config");
    std::ofstream file("config.json");
    nlohmann::json j = config;
    if (file)
    {
        file << j;
        file.close();
    }
    else
    {
        spdlog::warn("Failed to write config.json");
    }
}

void SpinWrap::makePathsAbsolute()
{
    config.spin_path     = fs::absolute(config.spin_path);
    config.compiler_path = fs::absolute(config.compiler_path);
    config.swarm_path    = fs::absolute(config.swarm_path);
    config.dot_path      = fs::absolute(config.dot_path);
    settings.trailPath   = fs::absolute(settings.trailPath);
}

bool SpinWrap::isConfigValid()
{
    return true;
}

Config &SpinWrap::mutConfig()
{
    return this->config;
}

Config const &SpinWrap::getConfig()
{
    return this->config;
}

SimulationSettings &SpinWrap::mutSettings()
{
    return this->settings;
}

SimulationSettings const &SpinWrap::getSettings()
{
    return this->settings;
}

bool SpinWrap::readFile(fs::path path, std::string &filecontent)
{
    spdlog::debug("Trying to open file {}", path.string());
    std::ifstream file(path);
    if (!file)
    {
        spdlog::error("Couldn't open file {}", path.string());
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    filecontent = buffer.str();

    return true;
}

// Editor
std::map<int, std::string> SpinWrap::generateErrorMarkers(std::string code)
{
    TempDir _(this->tmppath);
    std::map<int, std::string> markers;
    if (!this->isConfigValid())
    {
        spdlog::warn("No valid configuration found please check the settings tab");
        return markers;
    }

    static xp::sregex linenoRegex = xp::sregex::compile("(\\d+)");
    static xp::smatch what;
    std::ofstream tmpfile("tmp.pml");
    if (tmpfile)
    {
        tmpfile << code;
        tmpfile.close();
    }

    bp::ipstream std_out;
    bp::child c(config.spin_path.string(), "-a", "tmp.pml", bp::std_out > std_out);

    std::string line;
    int lastline = -1;
    std::string error_text;

    while (std_out && std::getline(std_out, line) && !line.empty())
    {
        int findError = line.find("Error:");
        if (findError == -1)
        {
            continue;
        }
        if (regex_search(line, what, linenoRegex))
        {
            if (what.size() <= 1 || line == "")
            {
                continue;
            }
            int currentline          = std::stoi(what[1].str());
            std::string currentError = std::string(line.substr(findError));
            if (lastline != currentline)
            {
                if (lastline != -1)
                {
                    markers.insert(std::make_pair<int, std::string>(int(lastline), std::string(error_text)));
                }
                error_text = currentError;
            }
            else
            {
                error_text += "\n" + currentError;
            }
            lastline = currentline;
        }
    }
    if (lastline != -1)
    {
        markers.insert(std::make_pair<int, std::string>(int(lastline), std::string(error_text)));
    }

    c.wait();

    // TODO: Parse warnings add them to TextEditor Repo and process preprocessor errors on stderr
    // TODO: Look into parsing messages from run and make config menu thing

    return markers;
}

// void generateSymbolTable(fs::path filepath)
// {
//     spdlog::info("Current Path {}", fs::current_path().string());
//     ipstream _stdout;
//     child c(config.spin_path, "-d", filepath.string(), std_out > _stdout);
//     std::string line;
//     while (_stdout && std::getline(_stdout, line) && !line.empty())
//     {
//         spdlog::info("{}");
//     }
// }

// void runProgram()
// {
// #ifdef PC
//     // FIXME Not working
//     if (config.compiler_path == "")
//     {
//         config.compiler_path  = locateVSExecutable("cl.exe");
//         config.vcvarsall_path = locateVSExecutable("vcvarsall.bat");
//         spdlog::info("MSVC: {}", config.compiler_path);
//         spdlog::info("MSVC: {}", config.vcvarsall_path);
//         fs::path filepath("../example.pml");
//         fs::path spinexe("extra/spin.exe");
//         fs::path backup = fs::current_path();
//         fs::current_path(config.tmppath);
//         ipstream _stdout;
//         system(config.vcvarsall_path + " x64; .\\" + spinexe.string(), std_out > _stdout);
//         std::string line;
//
//         int i = 0;
//         while (_stdout && std::getline(_stdout, line) && !line.empty())
//         {
//             spdlog::info("{}", line);
//         }
//         fs::current_path(backup);
//     }
// #endif
// }
