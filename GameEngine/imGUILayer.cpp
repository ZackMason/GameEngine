#include "imGUILayer.h"

#include "include/SDL2/SDL.h"
#include "imGUI/imgui.h"
#include "imGUI/imconfig.h"
#include "imGUI/imgui_internal.h"
#include "imGUI/imgui_impl_sdl.h"
#include "imGUI/imgui_impl_opengl3.h"
#include "Application.h"
#include "Config.h"



imGUILayer::imGUILayer()
{
}


imGUILayer::~imGUILayer()
{
}

void imGUILayer::OnAttach()
{
	auto  display = Application::Get().GetDisplay();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.DisplaySize = ImVec2(1920, 1080);
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(display->GetWindow(), display->GetContext());
	ImGui_ImplOpenGL3_Init("#version 430");
}

void imGUILayer::OnUpdate()
{
	auto  display = Application::Get().GetDisplay();
	auto c = Config::Get();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(display->GetWindow());
	ImGui::NewFrame();
	static float f = 0.0f;
	static int counter = 0;
	static ImVec4 clear_color(0, 0, 0, 0);

	ImGui::Begin(m_name.c_str());                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::SliderFloat("speed", &c.playerSpeed, 0.0f, 1000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("shine", &c.shine, 0.01f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("expos", &c.expos, 0.01f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("gamma", &c.gamma, 0.1f, 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("shader color", (float*)&c.shader_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imGUILayer::OnEvent()
{
}

void imGUILayer::OnDetach()
{
}
