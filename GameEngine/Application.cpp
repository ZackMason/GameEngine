#include "Application.h"


Application* Application::s_Instance = nullptr;

Application::Application() : 
m_display(nullptr),
m_level(nullptr)
{
	s_Instance = this;
}


Application::~Application()
{
}

void Application::OnUpdate()
{
	for (auto& layer : m_LayerStack)
		layer->OnUpdate();
	
	//SDL_GL_SwapWindow(m_display->GetWindow());
	m_display->Update();
}

void Application::OnStart()
{
}

void Application::OnExit()
{
}