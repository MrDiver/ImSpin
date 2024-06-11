#pragma once
#include "config.hpp"
#include <TextEditor.h>
#include <filesystem>
#include <map>
#include <string>
#include <variant>

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
    enum SymbolType
    {
        BIT,
        BYTE,
        SHORT,
        INT,
        CHAN,
    };
    enum Modifier
    {
        CONSTANT,
        VARIABLE
    };
    struct Primitive
    {
        SymbolType type;
        std::string name;
        uint32_t initial;
        bool global;
        std::string procName;
        std::string scope;
    };
    struct Array
    {
        SymbolType type;
        std::string name;
        uint32_t size;
        bool global;
        std::string procName;
        std::string scope;
    };
    struct Channel
    {
        uint32_t size;
        std::string name;
        bool global;
        std::string procName;
        std::string scope;
        std::vector<SymbolType> types;
    };
    struct Mtype
    {
        uint32_t size;
        std::string name;
        bool global;
        std::string procName;
        std::string scope;
        std::vector<SymbolType> types;
    };
    struct Proctype
    {
        uint32_t number;
        std::string name;
        bool global;
        std::string procName;
        std::string scope;
    };
    enum SymbolTag
    {
        PRIMITIVE,
        ARRAY,
        CHANNEL,
        MTYPE,
        PROCTYPE
    };
    typedef std::variant<Primitive, Array, Channel, Proctype> Symbol;

    SpinWrap();
    ~SpinWrap();
    // Config Functions
    void readConfigFile();
    void writeConfigFile();
    void makePathsAbsolute();
    bool isConfigValid();
    Config &mutConfig();
    Config const &getConfig();
    void configModified();
    SimulationSettings &mutSettings();
    SimulationSettings const &getSettings();
    static bool readFile(fs::path path, std::string &filecontent);
    // Editor
    void writeToTempFile(std::string code);
    std::map<int, std::string> generateErrorMarkers();
    std::pair<std::map<int, Symbol>, bool> generateSymbolTable();

  private:
    bool configValid = false;
    SimulationSettings settings;
    Config config;
    // Common
    std::filesystem::path tmppath;
};
