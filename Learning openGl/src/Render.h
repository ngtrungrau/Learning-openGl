#pragma once

#define ASSERT(x) if(!x) __debugbreak();
#define glCall(x) ClearGLError();\
                  x;\
                  ASSERT(glLogError(#x,__FILE__,__LINE__))

void ClearGLError();
bool glLogError(const char* function, const char* file, int line);

class VAO;
class IndexBuffer;
class Shader;

class Renderer
{
private:
    // State Caching để tránh re-bind không cần thiết
    mutable unsigned int m_CurrentlyBoundVAO = 0;
    mutable unsigned int m_CurrentlyBoundShader = 0;

public:
    // --- CLEAR ---
    // Cho phép chọn xóa Color, Depth hoặc cả hai (Mặc định xóa cả 2)
    void Clear() const;

    // --- BLEND STATE ---
    void EnableBlend() const;
    void DisableBlend() const;
    void SetBlendFunc(unsigned int src, unsigned int dst) const;
    void SetBlendEquation(unsigned int mode) const;

    // --- DEPTH TEST STATE ---
    void EnableDepthTest() const;
    void DisableDepthTest() const;

   
    // --- DRAW ---
    void Draw(const VAO& vao, const Shader& shader, unsigned int count, const void* start = nullptr) const;
};