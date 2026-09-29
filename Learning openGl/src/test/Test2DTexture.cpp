#include "Test2DTexture.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Render.h"
#include "Texture.h"
#include "VAO.h"
#include "imgui.h"
#include "VertexBufferLayout.h"

namespace test
{
	Test2DTexture::Test2DTexture() : m_vertices{
		// Top-Left
		{ {  560.0f, 240.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		// Top-Right
		{ { 1360.0f, 240.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		// Bottom-Right
		{ { 1360.0f, 840.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
		// Bottom-Left
		{ {  560.0f, 840.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }
	},
		m_indices{
			0, 1, 2,  // Tam giác 1
			0, 2, 3   // Tam giác 2
	}
	{

		m_VAO = std::make_unique<VAO>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_vertices, sizeof(m_vertices));
		m_IndexBuffer = std::make_unique<IndexBuffer>(m_indices, 6);
		m_VertexBufferLayout = std::make_unique< VertexBufferLayout>();
		m_VertexBufferLayout->AddLayout<float>(4, 0);
		m_VertexBufferLayout->AddLayout<float>(4, 0);
		m_VertexBufferLayout->AddLayout<float>(2, 0);
		m_VAO->AddVertexBuffer(*m_VertexBuffer, *m_IndexBuffer, *m_VertexBufferLayout);
		//////////////////////////////
		shader_path = "res/shader/BasicShader.shader";
		texture_path = "res/image/dog.png";
		last_shader_path = shader_path;
		last_texture_path = texture_path;
		memset(tmp_shader_path, 0, sizeof(tmp_shader_path));
		memset(tmp_texture_path, 0, sizeof(tmp_texture_path));
		strcpy_s(tmp_shader_path, shader_path.c_str());
		strcpy_s(tmp_texture_path, texture_path.c_str());

		
		m_Texture = std::make_unique<Texture>(texture_path);
		m_Texture->Bind(0);
		m_Shader = std::make_unique<Shader>(shader_path);
		m_Shader->Bind();
		m_Shader->SetUniform1i("uTexture", 0);
		m_Renderer = std::make_unique<Renderer>();
		m_Proj.Ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -2000.0f, 2000.0f);
		m_View.Identity();
		m_Model.Identity();

	};
	Test2DTexture::~Test2DTexture()
	{
	}
	void Test2DTexture::OnUpdate(float delta_Time)
	{
	}
	void Test2DTexture::OnRender()
	{
		if (last_shader_path != shader_path)
		{
			last_shader_path = shader_path;
			m_Shader = std::make_unique<Shader>(shader_path);
			m_Shader->Bind();
			m_Shader->SetUniform1i("uTexture", 0);
			m_Shader->UnBind();
		}
		if (last_texture_path != texture_path)
		{
			last_texture_path = texture_path;
			m_Texture = std::make_unique<Texture>(texture_path);
			m_Texture->Bind(0);
		}
		m_Shader->Bind();
		m_Shader->SetUniform4Mat("u_MVP", m_Proj * m_View * m_Model);
		m_Renderer->Draw(*m_VAO, *m_Shader, 6, (const void*)0);
	}
	void Test2DTexture::OnImGuiRender()
	{
		
		
		ImGui::InputText("Shader_path", tmp_shader_path, sizeof(tmp_shader_path));
		ImGui::SameLine();
		if (ImGui::Button("Conform##Shader"))
		{
			shader_path = tmp_shader_path;
		}
		ImGui::InputText("Texture_path", tmp_texture_path,sizeof(tmp_texture_path));
		ImGui::SameLine();
		if (ImGui::Button("Conform##Texture"))
		{
			texture_path = tmp_texture_path;
		}
		

		ImGui::DragFloat4("x", CoX.elements, 0.0001f);
		ImGui::DragFloat4("y", CoY.elements, 0.0001f);
		ImGui::DragFloat4("z", CoZ.elements, 0.0001f);
		ImGui::DragFloat4("w", CoW.elements, 0.0001f);
		m_Model.Columns[0] = { CoX.m_x,CoY.m_x,CoZ.m_x,CoW.m_x };
		m_Model.Columns[1] = { CoX.m_y,CoY.m_y,CoZ.m_y,CoW.m_y };
		m_Model.Columns[2] = { CoX.m_z,CoY.m_z,CoZ.m_z,CoW.m_z };
		m_Model.Columns[3] = { CoX.m_w,CoY.m_w,CoZ.m_w,CoW.m_w };

	}
}