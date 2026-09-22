#define STB_IMAGE_IMPLEMENTATION
#include"Render.h"
#include"..\res\stb_image\stb_image.h"
#include<string>
#include "Texture.h"
#include <GL/glew.h>
#include<GLFW/glfw3.h>

Texture::Texture(const std::string& path) :
	m_Renderer_ID(0), m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0), m_LocalBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	glCall(glGenTextures(1, &m_Renderer_ID));
	glCall(glBindTexture(GL_TEXTURE_2D, m_Renderer_ID));


	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	

	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
};

Texture::~Texture()
{
	glCall(glDeleteTextures(1, &m_Renderer_ID));
}



void Texture::Bind(unsigned int slot) const
{
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, m_Renderer_ID));
}

void Texture::UnBind() const
{
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
