#include "VAO.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include"Render.h"
#include"VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include<vector>
VAO::VAO()
{
	glCall(glGenVertexArrays(1, &m_Renderer_ID));
}

void VAO::AddVertexBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo, const VertexBufferLayout& layout)
{
	Bind();
	vbo.Bind();
	ibo.Bind();
	const auto& elements = layout.GetElements();
	unsigned int index = 0;
	unsigned int start = 0;
	for (const auto& it : elements)
	{
		glCall(glEnableVertexAttribArray(index));
		glCall(glVertexAttribPointer(index, it.count, it.type, it.normalised, layout.GetStride(), (const void*)(start)));
		start += it.getSize() * it.count;
		index++;
	}
	UnBind();

}



void VAO::Bind()const
{
	glCall(glBindVertexArray(m_Renderer_ID));
}

void VAO::UnBind()const
{
	glCall(glBindVertexArray(0));
}

VAO::~VAO()
{
	glCall(glDeleteVertexArrays(1, &m_Renderer_ID));
}


