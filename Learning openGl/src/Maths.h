#pragma once

class Vec2
{
public:
	union
	{
		// Truy cập theo tọa độ hoặc thành phần Texture (UV)
		struct { float m_x, m_y; };
		struct { float u, v; };
		struct { float width, height; };

		// Truy cập dạng mảng 1D
		float elements[2];
	};
	Vec2();
	Vec2(float x, float y);

	Vec2 operator+(const Vec2& other)const ;
	Vec2 operator-(const Vec2& other)const;

	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);

	float Length()const;
	Vec2& Normalize();
};
class Vec3
{
public:
	union
	{
		// Truy cập theo Không gian (XYZ) hoặc Màu sắc (RGB)
		struct { float m_x, m_y, m_z; };
		struct { float r, g, b; };

		// Ép kiểu/Trích xuất nhanh Vec2 (chỉ lấy XY)
		struct { Vec2 xy; float _pad0; };

		// Truy cập dạng mảng 1D
		float elements[3];
	};
	

	Vec3();
	Vec3(float x, float y,float z);

	
	Vec3 operator+(const Vec3& other)const;
	Vec3 operator-(const Vec3& other)const;
	Vec3 operator*(const float& other) const;
	
	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3& operator*=(const float& other);

	float Length()const;
	Vec3& Normalize();
	float Dot(const Vec3& v1, const Vec3& v2);
	Vec3 Cross(const Vec3& v1, const Vec3& v2);
};

class Vec4
{
public:
	union
	{
		// Truy cập theo Tọa độ (XYZW) hoặc Màu sắc (RGBA)
		struct { float m_x, m_y, m_z, m_w; };
		struct { float r, g, b, a; };

		// Ép kiểu/Trích xuất nhanh Vec3 hoặc Vec2
		struct { Vec3 xyz; float _pad0; };
		struct { Vec2 xy;  Vec2 zw; };

		// Truy cập dạng mảng 1D (dễ truyền vào Shader API)
		float elements[4];
	};

	Vec4();
	Vec4(float x, float y, float z,float w);
	Vec4 operator+(const Vec4& other)const;
	Vec4 operator-(const Vec4& other)const;

	Vec4& operator+=(const Vec4& other);
	Vec4& operator-=(const Vec4& other);
};
class Mat4 {
public:
	union
	{
		float Elements[16];


		float M[4][4];


		Vec4 Columns[4];

		// 4. Định danh rõ ràng từng cột (Cực kỳ tiện khi tính toán Transform/View matrix)
		struct
		{
			Vec4 Right;    // Cột 0
			Vec4 Up;       // Cột 1
			Vec4 Forward;  // Cột 2
			Vec4 Position; // Cột 3
		};
	};
	Mat4();
	Mat4& Identity(float diagonal = 1.0f);
	Mat4 operator*(const Mat4& other) const;
	Vec4 operator*(const Vec4& vec4) const;
	Vec3 TransformPoint(const Vec3& v)const;
	Vec3 TransformVector(const Vec3& v) const;
	Mat4& Translate(const Vec3& translation);
	Mat4& Scale(const Vec3& scale);
	Mat4& Rotate(float angleInDegrees, Vec3 axis);
	Mat4& Ortho(float left, float right, float bottom, float top, float near, float far);
	//Mat4 Inverse();

};
