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

	for (int number = 0; number < 200; number++)
	{
		auto n = std::to_string(number);
		std::vector<std::string> tmp;
		tmp.push_back( "pointLights[" + n + "].position");
		tmp.push_back( "pointLights[" + n + "].color");
		tmp.push_back( "pointLights[" + n + "].ambient");
		tmp.push_back( "pointLights[" + n + "].diffuse");
		tmp.push_back( "pointLights[" + n + "].specular");
		tmp.push_back( "pointLights[" + n + "].constant");
		tmp.push_back( "pointLights[" + n + "].linear");
		tmp.push_back( "pointLights[" + n + "].quadratic");
		light_names.push_back(tmp);
	}

}

void Application::OnExit()
{
}