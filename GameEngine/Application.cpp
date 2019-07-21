#include "Application.h"



Application::Application() : 
m_display(nullptr),
m_level(nullptr)
{

}


Application::~Application()
{
}

void Application::OnUpdate()
{
	if (!m_display->IsClosed())
		m_level->OnUpdate(*m_display, state);
	m_display->Update();

}

void Application::OnStart()
{
	m_level->OnStart();

}