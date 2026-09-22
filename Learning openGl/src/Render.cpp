#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Render.h"
#include "IndexBuffer.h"
#include"VAO.h"
#include"Shader.h"
void ClearGLError()
{
    while (glGetError() != GL_NO_ERROR);
}
bool glLogError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Open Gl Error!!] (" << error << ")" << " " << line << "(" << file << ")" << '\n';
        return false;
    }
    return true;
}
void Renderer::Clear() const
{
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void Renderer::EnableBlend() const
{
    glCall(glEnable(GL_BLEND));
}

void Renderer::DisableBlend() const
{
    glCall(glDisable(GL_BLEND));
}

void Renderer::SetBlendFunc(unsigned int src, unsigned int dst) const
{
    glCall(glBlendFunc(src, dst));
}

void Renderer::SetBlendEquation(unsigned int mode) const
{
    glCall(glBlendEquation(mode));
}

void Renderer::EnableDepthTest() const
{
    glCall(glEnable(GL_DEPTH_TEST));
    // Mặc định: Vật nằm gần camera hơn (Z nhỏ hơn) sẽ đè lên vật ở xa
    glCall(glDepthFunc(GL_LESS));
}

void Renderer::DisableDepthTest() const
{
    glCall(glDisable(GL_DEPTH_TEST));
}
void Renderer::Draw(const VAO& vao, const Shader& shader,unsigned int count,const void* start) const
{
    // Chỉ Bind Shader nếu nó chưa được Bind trước đó
    if (m_CurrentlyBoundShader != shader.GetID())
    {
        shader.Bind();
        m_CurrentlyBoundShader = shader.GetID();
    }

    // Chỉ Bind VAO nếu VAO này chưa được Bind
    if (m_CurrentlyBoundVAO != vao.GetID())
    {
        vao.Bind(); 
        m_CurrentlyBoundVAO = vao.GetID(); 
        
    }
    glCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, start));
}


