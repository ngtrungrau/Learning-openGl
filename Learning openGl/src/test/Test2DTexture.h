#pragma once
#include"Test.h"
#include<iostream>
#include"Maths.h"
#include "VertexBuffer.h"
#include <string>
class VAO;
class IndexBuffer;
class VertexBuffer;
class Shader;
class Renderer;
class Texture;
class VertexBufferLayout;
namespace test {
	class Test2DTexture : public Test
	{
	private:
		std::unique_ptr<VAO> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr< VertexBufferLayout> m_VertexBufferLayout;
		Vertex m_vertices[4];
		unsigned int m_indices[6];
		Mat4 m_Proj;
		Mat4 m_View;
		Mat4 m_Model;
		Vec4 CoX = { 1,0,0,0 };
		Vec4 CoY = { 0,1,0,0 };
		Vec4 CoZ = { 0,0,1,0 };
		Vec4 CoW = { 0,0,0,1 };
		std::string shader_path;
		std::string texture_path;
		std::string last_shader_path;
		std::string last_texture_path;
		char tmp_shader_path[128];
		char tmp_texture_path[128];
	public:
		Test2DTexture();
		~Test2DTexture();
		void OnUpdate(float delta_Time)override;
		void OnRender()override;
		void OnImGuiRender()override;


	};
}
