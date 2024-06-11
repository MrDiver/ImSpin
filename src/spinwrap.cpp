#include "spinwrap.hpp"
#include <boost/process.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;
namespace bp = boost::process;
namespace xp = boost::xpressive;

template <class... Ts> struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

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
    if (configValid)
    {
        return true;
    }
    bool valid = true;
    TempDir _(this->tmppath);
    if (!fs::exists(config.spin_path))
    {
        spdlog::warn("[Spin] Not found");
        valid = false;
    }
    else
    {
        bp::ipstream std_out;
        std::string line;
        bp::child c(config.spin_path.string(), "-V", bp::std_out > std_out);
        while (std_out && std::getline(std_out, line) && !line.empty())
        {
            spdlog::info("[Spin] {}", line);
        }
    }
    if (!fs::exists(config.swarm_path))
    {
        spdlog::warn("[Swarm] Not found");
        valid = false;
    }
    else
    {
        bp::ipstream std_out;
        std::string line;
        bp::child c(config.swarm_path.string(), "-V", bp::std_out > std_out);
        while (std_out && std::getline(std_out, line) && !line.empty())
        {
            spdlog::info("[Swarm] {}", line);
        }
    }
    try
    {
        bp::ipstream std_out;
        std::string line;
        bp::child c(config.compiler_path.string() + " --version", bp::std_out > std_out);
        while (std_out && std::getline(std_out, line) && !line.empty())
        {
            spdlog::info("[CC] {}", line);
        }
    }
    catch (std::exception e)
    {
        spdlog::warn("[CC] Compiler Command does not work");
    }
    configValid = valid;
    return valid;
}

Config &SpinWrap::mutConfig()
{
    return this->config;
}

Config const &SpinWrap::getConfig()
{
    return this->config;
}

void SpinWrap::configModified()
{
    this->configValid = true;
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
std::map<int, std::string> SpinWrap::generateErrorMarkers()
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

    bp::ipstream std_out;
    bp::child c(config.spin_path.string(), "-a", "tmp.pml", bp::std_out > std_out);

    std::string line;
    int lastline = -1;
    std::string error_text;

    while (std_out && std::getline(std_out, line) && !line.empty())
    {
        spdlog::info(line);
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

bool findModifiers(std::string const &token, bool &globalIdx, bool &variableIdx, bool &constantIdx)
{
    globalIdx   = -1;
    variableIdx = 1;
    globalIdx   = false;
    variableIdx = false;
    constantIdx = false;
    bool found  = false;

    if (token.find("global") != -1)
    {
        found     = true;
        globalIdx = true;
    }
    if (token.find("variable") != -1)
    {
        found       = true;
        variableIdx = true;
    }
    if (token.find("constant") != -1)
    {
        found       = true;
        constantIdx = true;
    }

    return found;
}

SpinWrap::Symbol parsePrimitive(std::vector<std::string> const &tokens)
{
    SpinWrap::Primitive p;
    if (tokens.at(0) == "bit")
        p.type = SpinWrap::BIT;
    else if (tokens.at(0) == "byte")
        p.type = SpinWrap::BYTE;
    else if (tokens.at(0) == "short")
        p.type = SpinWrap::SHORT;
    else if (tokens.at(0) == "int")
        p.type = SpinWrap::INT;
    uint32_t i = 1;
    p.name     = tokens.at(i++);
    p.initial  = std::stoi(tokens.at(i++));
    bool isGlobal, isVariable, isConstant;

    for (uint32_t idx = i; i < tokens.size(); i++)
    {
        bool foundMod = findModifiers(tokens.at(idx), isGlobal, isVariable, isConstant);
        // Check if process name
        if (!foundMod && idx == i)
        {
            p.procName = tokens.at(idx).substr(1, tokens.at(idx).size() - 2);
        }
        if (foundMod)
        {
            p.global |= isGlobal;
        }
    }

    std::string scope = tokens.at(tokens.size() - 1);
    p.scope           = scope.substr(scope.find_first_of('_'), scope.size() - 1); // slice _X_....

    // Parsing initial value, this might also be the channel size or the line number in a process for labels
    SpinWrap::Symbol s = p;
    return s;
}

std::pair<std::map<int, SpinWrap::Symbol>, bool> SpinWrap::generateSymbolTable()
{
    TempDir _(this->tmppath);
    std::map<int, SpinWrap::Symbol> markers;
    std::vector<SpinWrap::Symbol> symbols;
    if (!this->isConfigValid())
    {
        spdlog::warn("No valid configuration found please check the settings tab");
        return {markers, false};
    }

    bp::ipstream std_out;
    bp::child c(config.spin_path.string(), "-d", "tmp.pml", bp::std_out > std_out);
    c.wait();
    if (c.exit_code())
    {
        return {markers, false};
    }

    std::string line;
    std::string error_text;

    static xp::sregex word_regex = xp::sregex::compile("(\\w+)");
    static xp::smatch what;
    std::vector<std::string> tokens;
    tokens.reserve(10);
    while (std_out && std::getline(std_out, line) && !line.empty())
    {
        tokens.clear();
        Symbol sym;
        auto it = xp::sregex_token_iterator(line.begin(), line.end(), word_regex);
        xp::sregex_token_iterator end;
        for (; it != end; it++)
        {
            tokens.push_back(*it);
        }

        spdlog::info("|{}|", tokens.at(0));
        if (tokens.at(0) == "bit" || tokens.at(0) == "byte" || tokens.at(0) == "short" || tokens.at(0) == "int")
        {
            symbols.push_back(parsePrimitive(tokens));
        }
        else if (tokens.at(0) == "chan")
        {
        }

        // TODO: Figure out first occurence of symbols

        else if (tokens.at(0) == "proctype")
        {
        }

        spdlog::info("SymbolSize: {}", symbols.size());
        for (auto const &symbol : symbols)
        {
            std::visit(overloaded{[](Primitive arg) { spdlog::info("Primitive Name: {}", arg.name); }, [](auto arg) {}},
                       symbol);
        }
    }

    return {markers, true};
}

void SpinWrap::writeToTempFile(std::string code)
{
    TempDir _(this->tmppath);
    std::ofstream tmpfile("tmp.pml");
    if (tmpfile)
    {
        tmpfile << code;
        tmpfile.close();
    }
}
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
