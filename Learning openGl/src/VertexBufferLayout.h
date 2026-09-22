#pragma once
#include <vector>
#include <GL/glew.h>
#include<iostream>
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalised;
	unsigned int getSize() const;

};
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() :m_Stride(0) {};
	template<typename Type>
	inline void AddLayout(unsigned int count,unsigned char normalised)
	{
		static_assert(sizeof(Type) == 0, "Can't add layout with the wrong type!\n");
	}
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
template<>
inline void VertexBufferLayout::AddLayout<float>(unsigned int count, unsigned char normalised)
{
	VertexBufferElement element = { GL_FLOAT,count,normalised };
	m_Stride += count * element.getSize();
	m_Elements.push_back(std::move(element));

}
template<>
inline void VertexBufferLayout::AddLayout<unsigned int>(unsigned int count, unsigned char normalised)
{
	VertexBufferElement element = { GL_UNSIGNED_INT,count,normalised };
	m_Stride += count * element.getSize();
	m_Elements.push_back(std::move(element));

}
template<>
inline void VertexBufferLayout::AddLayout<unsigned char>(unsigned int count, unsigned char normalised)
{
	VertexBufferElement element = { GL_UNSIGNED_BYTE,count,normalised };
	m_Stride += count * element.getSize();
	m_Elements.push_back(std::move(element));

}
