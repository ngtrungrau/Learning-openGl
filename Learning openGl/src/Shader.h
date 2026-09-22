#pragma once

#include <string>
#include <unordered_map>

// Forward declarations (Nếu Maths.h đã bao gồm đủ thì có thể giữ hoặc bỏ)
class Vec3;
class Vec4;
class Mat4;

struct ShaderProgramSource
{
    std::string VertexShader;
    std::string FragmentShader;
};

enum class ShaderType : int
{
    Vertex = 0,
    Fragment = 1,
    None = -1
};

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_Renderer_ID;

    // Cache lưu vị trí của các Uniform
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    // --- SET UNIFORMS ---
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform3f(const std::string& name, const Vec3& v);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform4f(const std::string& name, const Vec4& v);
    void SetUniform4Mat(const std::string& name, const Mat4& mat4);

    // --- GET UNIFORMS (Trả về giá trị trực tiếp - Không cần biến hứng) ---
    int   GetUniform1i(const std::string& name);
    float GetUniform1f(const std::string& name);
    Vec3  GetUniform3f(const std::string& name);
    Vec4  GetUniform4f(const std::string& name);
    Mat4  GetUniform4Mat(const std::string& name);

    inline unsigned int GetID() const { return m_Renderer_ID; }

private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};