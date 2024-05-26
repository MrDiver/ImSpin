#include <glad/gl.h>
//
#include "ezgl.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace ez
{
GLint getGLTypeSize(GLenum type)
{
    switch (type)
    {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        return sizeof(uint8_t);
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
        return sizeof(uint16_t);
    case GL_INT:
    case GL_UNSIGNED_INT:
        // case GL_FIXED:
        return sizeof(uint32_t);
    case GL_FLOAT:
        return sizeof(float);
    case GL_DOUBLE:
        return sizeof(double);
    }
    spdlog::error("Could not find size of type {} for GL Types", type);
    exit(EXIT_FAILURE);
}

void checkError()
{
    switch (glGetError())
    {
    case GL_NO_ERROR:
        break;
    case GL_INVALID_ENUM:
        spdlog::error("Invalid Enum");
        break;
    case GL_INVALID_VALUE:
        spdlog::error("Invalid Value");
        break;
    case GL_INVALID_OPERATION:
        spdlog::error("Invalid Operation");
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        spdlog::error("Invalid Framebuffer Operation");
        break;
    case GL_OUT_OF_MEMORY:
        spdlog::error("Out of Memory");
        break;
    case GL_STACK_UNDERFLOW:
        spdlog::error("Stack Underflow");
        break;
    case GL_STACK_OVERFLOW:
        spdlog::error("Stack Overflow");
        break;
    }
}

/* VertexBuffer */

VertexBuffer::VertexBuffer(bool dynamic)
{
    glGenBuffers(1, &this->id);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

/* VertexArray */
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &this->id);
    this->bind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->id);
}

void VertexArray::bind()
{
    glBindVertexArray(this->id);
}

void VertexArray::attributes(std::initializer_list<std::pair<GLenum, GLint>> elements)
{
    spdlog::debug("Setting attributes for {}", this->id);
    GLint stride = 0;
    for (auto const &p : elements)
    {
        stride += getGLTypeSize(p.first) * p.second;
    }

    size_t offset = 0;
    GLint counter = 0;
    for (auto const &p : elements)
    {
        GLint size = getGLTypeSize(p.first) * p.second;
        glVertexAttribPointer(counter, p.second, p.first, GL_FALSE, stride, (GLvoid *)offset);
        glEnableVertexAttribArray(counter);

        spdlog::debug("VAO {}: Attr: {} size: {} type: {} stride: {} offset: {}", this->id, counter, p.second, p.first,
                      stride, offset);

        offset += size;
        counter++;
    }
}

/* Program */

std::string readFile(std::string_view path, std::vector<std::string> &includes)
{
    spdlog::debug("Reading file {}", path.data());
    std::ifstream stream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (not stream)
    {
        spdlog::error("File {} does not exist", path);
        exit(EXIT_FAILURE);
    }

    fs::path p(path);
    #ifdef _WIN32
    includes.push_back(p.filename().string());
    #else
    includes.push_back(p.filename());
    #endif
    p = p.remove_filename();

    std::string content;
    std::string line;
    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            if (line.starts_with("#include"))
            {
                size_t start = line.find("\"");
                size_t end = line.find("\"", start + 1);
                std::string sub(line.begin() + start + 1, line.begin() + end);
                spdlog::debug("Found include with name {} attempting read", p.string() + sub);
                std::string subfile = readFile(p.string() + sub, includes);
                content += "// === START INCLUDE " + sub + " ===\n";
                content += subfile;
                content += "// === END INCLUDE " + sub + " ===\n";
            }
            else
            {
                content += line + "\n";
            }
        }
        stream.close();
    }
    else
    {
        spdlog::error("Unable to open file {}", path.data());
    }
    spdlog::debug("{}", content);

    return content;
}

Program::Program(std::string const &vertexPath, std::string const &fragmentPath, bool autoreload)
    : autoreload(autoreload), vertexPath(vertexPath), fragmentPath(fragmentPath)
{
    this->compile();
}

Program::~Program()
{
    glDeleteProgram(this->id);
}

void Program::compile()
{
    needsRecompile = false;
    includedFiles.clear();

    std::string vertexSource = readFile(this->vertexPath, this->includedFiles);
    std::string fragmentSource = readFile(this->fragmentPath, this->includedFiles);
    /* spdlog::info("{}", vertexSource); */
    /* spdlog::info("{}", fragmentSource); */
    for (auto const &e : this->includedFiles)
    {
        spdlog::debug("Includes {}", e);
    }
    GLint vertShader, fragShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];

    const char *tmp = vertexSource.c_str();
    glShaderSource(vertShader, 1, &tmp, NULL);
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        spdlog::error("VertexShader compilation failed \n{}", infoLog);
        return;
    }

    tmp = fragmentSource.c_str();
    glShaderSource(fragShader, 1, &tmp, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        spdlog::error("FragmentShader compilation failed \n{}", infoLog);
        return;
    }

    if (this->id)
    {
        glUseProgram(0);
        glDeleteProgram(this->id);
    }
    this->id = glCreateProgram();
    if (this->id == 0)
    {
        auto e = glGetError();
        spdlog::error("glCreateProgram returned zero {}", e);
        exit(EXIT_FAILURE);
    }

    glAttachShader(this->id, vertShader);
    glAttachShader(this->id, fragShader);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        spdlog::error("Linking Program failed \n{}", infoLog);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    spdlog::info("Recompiled shaders");
}

void Program::recompile()
{
    this->needsRecompile = true;
}

void Program::use()
{
    if (needsRecompile)
    {
        this->compile();
    }
    glUseProgram(this->id);
}

void Program::setInt(std::string const &name, uint32_t value)
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}
void Program::setFloat(std::string const &name, float value)
{
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}
void Program::setVec2(std::string const &name, glm::vec2 const &value)
{
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y);
}
void Program::setVec2(std::string const &name, float v1, float v2)
{
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), v1, v2);
}
void Program::setVec3(std::string const &name, glm::vec3 const &value)
{
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z);
}
void Program::setVec3(std::string const &name, float v1, float v2, float v3)
{
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3);
}
void Program::setVec4(std::string const &name, glm::vec4 const &value)
{
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z, value.w);
}
void Program::setVec4(std::string const &name, float v1, float v2, float v3, float v4)
{
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3, v4);
}

void Program::setDouble(std::string const &name, double value)
{
    glUniform1d(glGetUniformLocation(this->id, name.c_str()), value);
}
void Program::setDVec2(std::string const &name, glm::dvec2 const &value)
{
    glUniform2d(glGetUniformLocation(this->id, name.c_str()), value.x, value.y);
}
void Program::setDVec3(std::string const &name, glm::dvec3 const &value)
{
    glUniform3d(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z);
}
void Program::setDVec3(std::string const &name, double v1, double v2, double v3)
{
    glUniform3d(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3);
}
void Program::setDVec4(std::string const &name, glm::dvec4 const &value)
{
    glUniform4d(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z, value.w);
}
void Program::setDVec4(std::string const &name, double v1, double v2, double v3, double v4)
{
    glUniform4d(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3, v4);
}

SSBO::SSBO()
{
    glGenBuffers(1, &this->id);
}
SSBO::~SSBO()
{
    glDeleteBuffers(1, &this->id);
}
void SSBO::bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
}
void SSBO::layout(GLint binding)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, this->id);
}

Texture2D::Texture2D(GLint width, GLint height) : width(width), height(height)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGenTextures(1, &this->id);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &this->id);
}

void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::setData(unsigned char data[])
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

} // namespace ez
