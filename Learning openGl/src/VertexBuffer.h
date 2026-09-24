#pragma once
struct Vertex
{
    struct
    {
        float x, y, z,w;
    };
    struct
    {
        float r, g, b,a;
    };
    struct {
        float U, V;
    };
};
class VertexBuffer {

private:
    unsigned int m_Renderer_ID;
public:
    VertexBuffer(const void* data, unsigned int size);
    void Bind() const;
    void unBind() const;
    ~VertexBuffer();
    inline unsigned int GetID() const;


};
inline unsigned int VertexBuffer::GetID() const
{
    return m_Renderer_ID;
}