#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application() : 
m_display(nullptr)
{
	s_Instance = this;
}


void Application::OnUpdate()
{
	for (auto& layer : m_LayerStack)
		layer->OnUpdate();
	
	m_display->Update();
}

void Application::OnStart()
{
}

void Application::OnExit()
{
	for (auto & layer : m_LayerStack)
		delete layer;
}