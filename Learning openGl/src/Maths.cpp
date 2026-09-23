#include "Maths.h"
#include <cmath>

// --- VEC2 ---
Vec2::Vec2() : m_x(0), m_y(0) {}
Vec2::Vec2(float x, float y) : m_x(x), m_y(y) {}

Vec2 Vec2::operator+(const Vec2& other) const { return { m_x + other.m_x, m_y + other.m_y }; }
Vec2 Vec2::operator-(const Vec2& other) const { return { m_x - other.m_x, m_y - other.m_y }; }

Vec2& Vec2::operator+=(const Vec2& other) { m_x += other.m_x; m_y += other.m_y; return *this; }
Vec2& Vec2::operator-=(const Vec2& other) { m_x -= other.m_x; m_y -= other.m_y; return *this; }

float Vec2::Length() const { return std::sqrt(m_x * m_x + m_y * m_y); }

Vec2& Vec2::Normalize()
{
    float len = Length();
    if (len > 0.0f) { m_x /= len; m_y /= len; }
    return *this;
}

// --- VEC3 ---
Vec3::Vec3() : m_x(0), m_y(0), m_z(0) {}
Vec3::Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

Vec3::Vec3(const float datas[3])
{
    elements[0] = datas[0];
    elements[1] = datas[1];
    elements[2] = datas[2];
}

Vec3 Vec3::operator+(const Vec3& other) const { return { m_x + other.m_x, m_y + other.m_y, m_z + other.m_z }; }
Vec3 Vec3::operator-(const Vec3& other) const { return { m_x - other.m_x, m_y - other.m_y, m_z - other.m_z }; }
Vec3 Vec3::operator*(const float& other) const { return { m_x * other, m_y * other, m_z * other }; }

Vec3& Vec3::operator+=(const Vec3& other) { m_x += other.m_x; m_y += other.m_y; m_z += other.m_z; return *this; }
Vec3& Vec3::operator-=(const Vec3& other) { m_x -= other.m_x; m_y -= other.m_y; m_z -= other.m_z; return *this; }

Vec3& Vec3::operator*=(const float& other)
{
    m_x *= other; m_y *= other; m_z *= other;
    return *this;
}

float Vec3::Length() const { return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }

Vec3& Vec3::Normalize()
{
    float len = Length();
    if (len > 0.0f) { m_x /= len; m_y /= len; m_z /= len; }
    return *this;
}

// Sửa lại: Dot Product chuẩn không chia cho Length
float Vec3::Dot(const Vec3& v1, const Vec3& v2)
{
    return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z;
}

Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2)
{
    return {
        v1.m_y * v2.m_z - v1.m_z * v2.m_y,
        v1.m_z * v2.m_x - v1.m_x * v2.m_z,
        v1.m_x * v2.m_y - v1.m_y * v2.m_x
    };
}

// --- VEC4 ---
Vec4::Vec4() : m_x(0), m_y(0), m_z(0), m_w(0) {}
Vec4::Vec4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

Vec4::Vec4(const float datas[4]) {
    elements[0] = datas[0];
    elements[1] = datas[1];
    elements[2] = datas[2];
    elements[3] = datas[3];
    

}
Vec4 Vec4::operator+(const Vec4& other) const { return { m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w }; }
Vec4 Vec4::operator-(const Vec4& other) const { return { m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w }; }

Vec4& Vec4::operator+=(const Vec4& other) { m_x += other.m_x; m_y += other.m_y; m_z += other.m_z; m_w += other.m_w; return *this; }
Vec4& Vec4::operator-=(const Vec4& other) { m_x -= other.m_x; m_y -= other.m_y; m_z -= other.m_z; m_w -= other.m_w; return *this; }

// --- MAT4 ---
Mat4::Mat4()
{
    for (int i = 0; i < 16; ++i) Elements[i] = 0.0f;
}

// Sửa tên thành Identity
Mat4& Mat4::Identity(float diagonal)
{
    for (int i = 0; i < 16; ++i) Elements[i] = 0.0f;
    Columns[0].m_x = diagonal;
    Columns[1].m_y = diagonal;
    Columns[2].m_z = diagonal;
    Columns[3].m_w = diagonal;
    return *this;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 tmp;
    for (int i = 0; i < 4; i++)
    {
        tmp.Columns[i] = (*this) * other.Columns[i];
    }
    return tmp;
}

// Sửa phép nhân Mat4 * Vec4 chuẩn Column-Major
Vec4 Mat4::operator*(const Vec4& other) const
{
    return Vec4(
        Columns[0].m_x * other.m_x + Columns[1].m_x * other.m_y + Columns[2].m_x * other.m_z + Columns[3].m_x * other.m_w,
        Columns[0].m_y * other.m_x + Columns[1].m_y * other.m_y + Columns[2].m_y * other.m_z + Columns[3].m_y * other.m_w,
        Columns[0].m_z * other.m_x + Columns[1].m_z * other.m_y + Columns[2].m_z * other.m_z + Columns[3].m_z * other.m_w,
        Columns[0].m_w * other.m_x + Columns[1].m_w * other.m_y + Columns[2].m_w * other.m_z + Columns[3].m_w * other.m_w
    );
}

// Sửa bổ sung return *this;
Mat4& Mat4::Translate(const Vec3& translation)
{
    Columns[3].m_x += translation.m_x;
    Columns[3].m_y += translation.m_y;
    Columns[3].m_z += translation.m_z;
    return *this;
}

Mat4& Mat4::Scale(const Vec3& scale)
{
    Columns[0].m_x *= scale.m_x;
    Columns[1].m_y *= scale.m_y;
    Columns[2].m_z *= scale.m_z;
    return *this;
}

// Hàm Ortho hoàn chỉnh
Mat4& Mat4::Ortho(float left, float right, float bottom, float top, float near, float far)
{
    Identity(1.0f);

    float a = 2.0f / (right - left);
    float b = -(right + left) / (right - left); // Tương đương 1 - (2*right)/(right-left)

    float c = 2.0f / (top - bottom);
    float d = -(top + bottom) / (top - bottom);

    float e = -2.0f / (far - near);             // Đảo dấu âm cho OpenGL
    float f = -(far + near) / (far - near);

    Columns[0].m_x = a;
    Columns[1].m_y = c;
    Columns[2].m_z = e;
    Columns[3] = { b, d, f, 1.0f };

    return *this;
}
/*
Mat4 Mat4::Inverse()
{
    Mat4 tmp;
    tmp.Columns[0].
   
}
*/