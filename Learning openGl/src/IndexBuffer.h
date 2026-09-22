#pragma once
class IndexBuffer {

private:
    unsigned int m_Renderer_ID;
    unsigned int m_Count;
public:
    IndexBuffer(unsigned int* data, unsigned int count);
    void Bind() const;
    void unBind() const;
    ~IndexBuffer();
    inline unsigned int GetID() const;
    inline unsigned int GetCount()const;

};
inline unsigned int IndexBuffer::GetID() const
{
    return m_Renderer_ID;
}

