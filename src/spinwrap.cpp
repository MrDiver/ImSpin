#include "spinwrap.hpp"
#include "TextEditor.h"
#include "config.hpp"
#include <boost/process.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <strstream>

namespace spin
{
using namespace boost::process;
using namespace boost::xpressive;
namespace fs = std::filesystem;
TextEditor::ErrorMarkers markers;
sregex linenoRegex = sregex::compile("(\\d+)");
smatch what;
fs::path tmppath = fs::temp_directory_path();
std::string tmpfilepath = (tmppath / "tmp.pml").string();

TextEditor::ErrorMarkers generateErrorMarkers(std::string code)
{
    markers.clear();
    std::ofstream tmpfile(tmpfilepath);
    if (tmpfile)
    {
        tmpfile << code;
        tmpfile.close();
    }

    ipstream pipe_stream;
    child c(config.spin_path + " " + tmpfilepath, std_out > pipe_stream);

    std::string line;
    int lastline = -1;
    std::string error_text;

    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    {
        int findError = line.find("Error:");
        spdlog::info("Line {} {}", line, findError);
        if (findError == -1)
        {
            break;
        }
        if (regex_search(line, what, linenoRegex))
        {
            if (what.size() <= 1 || line == "")
            {
                break;
            }
            int currentline = std::stoi(what[1].str());
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

std::string formatFile(std::string code)
{

    ipstream stdout, stderr;
    opstream stdin;
    child c(config.spin_path, "-pp", std_out > stdout, std_err > stderr, std_in < stdin);

    std::string line;

    stdin << code;
    stdin.pipe().close();

    std::strstream buf;
    while (stdout && std::getline(stdout, line) && !line.empty())
    {
        buf << line << "\n";
    }
    c.wait();

    return std::string(buf.str());
}
} // namespace spin
