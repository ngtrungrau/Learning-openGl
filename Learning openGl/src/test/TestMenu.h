#pragma once
#include"Test.h"
#include<vector>
#include<string>
#include<functional>
#include<iostream>
namespace test
{
	class TestMenu : public Test
	{
	private:
		Test* m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	public:
		TestMenu(Test*& CurrentTest);
		~TestMenu();
		template<typename T>
		void RegisterTest(const std::string& name);
	    void OnUpdate(float delta_Time) override;
		void OnRender()override;
		void OnImGuiRender() override;
		Test* GetCurrentTest()const;
		void SetCurrentTest(Test* currentTest);
		void DeleteCurrentTest();


	};
	template<typename T>
	inline void TestMenu::RegisterTest(const std::string& name)
	{
		std::cout << "Register test: " << name << '\n';
		m_Tests.push_back({ name,[]() {
			return new T();
		} });
	}
	
}