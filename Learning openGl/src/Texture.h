#pragma once
#include <string> 

class Texture
{
private:
    unsigned int m_Renderer_ID;
    std::string m_FilePath;      

    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

public:
   
    Texture(const std::string& path);
    ~Texture();

   

    void Bind(unsigned int slot = 0) const; // Thường bind kèm texture slot (mặc định là 0)
    void UnBind() const;

    // Các hàm Getters để lấy thông tin kích thước texture
    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};