#pragma once
class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;
class VAO
{
private:
	unsigned int m_Renderer_ID;
public:
	VAO();
	void AddVertexBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo, const VertexBufferLayout& layout);
	void Bind()const;
	void UnBind()const;
	~VAO();
	inline unsigned int GetID() const;
};
inline unsigned int VAO::GetID() const
{
	return m_Renderer_ID;
}