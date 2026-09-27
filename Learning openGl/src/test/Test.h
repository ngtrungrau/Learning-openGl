#pragma once
namespace test
{
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};
		virtual void OnUpdate(float delta_Time)=0;
		virtual void OnRender()=0;
		virtual void OnImGuiRender() = 0;
	};
}