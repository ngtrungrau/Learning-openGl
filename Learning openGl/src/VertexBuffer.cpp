#include "VertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"Render.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glCall(glGenBuffers(1, &m_Renderer_ID));
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	unBind();
}

void VertexBuffer::Bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID));
}

void VertexBuffer::unBind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER,0));
}

VertexBuffer::~VertexBuffer()
{
	glCall(	glDeleteBuffers(1, &m_Renderer_ID));
}

