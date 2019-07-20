#include "Application.h"



Application::Application() : 
m_display("Tripfire",1920,1080),
m_level(m_display)
{

}


Application::~Application()
{
}

void Application::OnUpdate()
{
	if (!m_display.IsClosed())
		m_level.OnUpdate(m_display, state);
	m_display.Update();

}

void Application::OnStart()
{
	m_level.OnStart();

}