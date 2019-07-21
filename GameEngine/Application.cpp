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
	if (!m_display->IsClosed())
		m_level->OnUpdate(*m_display, state);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_display->GetWindow());
	ImGui::NewFrame();
	static float f = 0.0f;
	static int counter = 0;
	static ImVec4 clear_color(0, 0, 0, 0);

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_display->GetWindow());
	m_display->Update();

}

void Application::OnStart()
{
	m_level->OnStart();

}