#pragma once
#include"Test.h"
namespace test
{
	class TestClearColor:public Test
	{
	private:
		float m_ClearColor[4];
	public:
		TestClearColor();
		~TestClearColor()override;
		virtual void OnUpdate(float delta_Time)override;
		virtual void OnRender()override;
		virtual void OnImGuiRender()override;

	};
}