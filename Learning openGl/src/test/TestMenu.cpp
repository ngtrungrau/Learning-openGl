#include "TestMenu.h"
#include "imgui.h"

test::TestMenu::TestMenu(Test*& CurrentTest):m_CurrentTest(CurrentTest) {};


test::TestMenu::~TestMenu()
{
}

void test::TestMenu::OnUpdate(float delta_Time)
{
	if (m_CurrentTest!=this)
	m_CurrentTest->OnUpdate(delta_Time);
}

void test::TestMenu::OnRender()
{
	if (m_CurrentTest!=this)
	m_CurrentTest->OnRender();
}

void test::TestMenu::OnImGuiRender()
{
	for (auto& it : m_Tests)
	{
		if (ImGui::Button(it.first.c_str()))
		{
			m_CurrentTest= it.second();
		}
	}
}

test::Test* test::TestMenu::GetCurrentTest()const
{
	return m_CurrentTest;
}

void test::TestMenu::SetCurrentTest(Test* currentTest)
{
	m_CurrentTest = currentTest;
}

void test::TestMenu::DeleteCurrentTest()
{
	delete m_CurrentTest;
}
