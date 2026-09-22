#include "IndexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"Render.h"
IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count):
	m_Count(count)
{
	glCall(glGenBuffers(1, &m_Renderer_ID));
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
	unBind();
}

void IndexBuffer::Bind()const 
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
}

void IndexBuffer::unBind()const
{
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer() 
{
	glCall(glDeleteBuffers(1, &m_Renderer_ID));
}

