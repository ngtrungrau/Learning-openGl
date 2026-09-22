#include "Shader.h"
#include <GL/glew.h>
#include "Render.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Maths.h"
Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_Renderer_ID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_Renderer_ID = CreateShader(source.VertexShader, source.FragmentShader);

    // Đã xóa dòng khai báo biến m_UniformLocationCache cục bộ gây shadowing lỗi cache!
}

Shader::~Shader()
{
    glCall(glDeleteProgram(m_Renderer_ID));
}

void Shader::Bind() const
{
    glCall(glUseProgram(m_Renderer_ID));
}

void Shader::UnBind() const
{
    glCall(glUseProgram(0));
}

// ==========================================
// SET UNIFORMS
// ==========================================

void Shader::SetUniform1i(const std::string& name, int value)
{
    glCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3f(const std::string& name, const Vec3& v)
{
    glCall(glUniform3f(GetUniformLocation(name), v.m_x, v.m_y, v.m_z));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, const Vec4& v)
{
    glCall(glUniform4f(GetUniformLocation(name), v.m_x, v.m_y, v.m_z, v.m_w));
}

void Shader::SetUniform4Mat(const std::string& name, const Mat4& mat4)
{
    // Tự động lấy địa chỉ phần tử float đầu tiên của struct Mat4
    glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat4.Columns[0].m_x));
}

// ==========================================
// GET UNIFORMS (Return Trực Tiếp - Chuẩn C++)
// ==========================================

int Shader::GetUniform1i(const std::string& name)
{
    int value = 0;
    glCall(glGetUniformiv(m_Renderer_ID, GetUniformLocation(name), &value));
    return value;
}

float Shader::GetUniform1f(const std::string& name)
{
    float value = 0.0f;
    glCall(glGetUniformfv(m_Renderer_ID, GetUniformLocation(name), &value));
    return value;
}

Vec3 Shader::GetUniform3f(const std::string& name)
{
    Vec3 result;
    glCall(glGetUniformfv(m_Renderer_ID, GetUniformLocation(name), &result.m_x));
    return result;
}

Vec4 Shader::GetUniform4f(const std::string& name)
{
    Vec4 result;
    glCall(glGetUniformfv(m_Renderer_ID, GetUniformLocation(name), &result.m_x));
    return result;
}

Mat4 Shader::GetUniform4Mat(const std::string& name)
{
    Mat4 result;
    glCall(glGetUniformfv(m_Renderer_ID, GetUniformLocation(name), &result.Columns[0].m_x));
    return result;
}

// ==========================================
// PARSE & COMPILE SHADER
// ==========================================

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType Type = ShaderType::None;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos) Type = ShaderType::Vertex;
            else if (line.find("fragment") != std::string::npos) Type = ShaderType::Fragment;
        }
        else
        {
            if (Type != ShaderType::None)
                ss[static_cast<int>(Type)] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string mess;
        mess.resize(length);
        glGetShaderInfoLog(id, length, &length, mess.data());

        std::cout << "[Shader Error] Lỗi biên dịch "
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << " Shader!" << std::endl;
        std::cout << mess << std::endl;

        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (vs == 0 || fs == 0) return 0;

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    int location = -1;
    glCall(location = glGetUniformLocation(m_Renderer_ID, name.c_str()));

    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}