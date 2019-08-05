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

	ImGui::Begin(m_name.c_str());                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Welcome.");               // Display some text (you can use a format strings too)

	ImGui::SliderFloat("speed", &c.playerSpeed, 0.0f, 1000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("shine", &c.shine, 0.01f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("expos", &c.expos, 0.01f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("gamma", &c.gamma, 0.0001f, 2.20f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("bias", &c.bias, 0.00001f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("scale", &c.scale, 0.000001f, 6.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("power", &c.power, 0.1f, 17.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("lp", &c.lp, 0.f, 1.f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("shader color", (float*)&c.shader_color);  // Edit 3 floats representing a color
	ImGui::ColorEdit3("shader color2", (float*)&c.shader_color2);  // Edit 3 floats representing a color
	ImGui::ColorEdit3("shader color3", (float*)&c.shader_color3);  // Edit 3 floats representing a color
	ImGui::ColorEdit3("shader color4", (float*)&c.shader_color4);  // Edit 3 floats representing a color

	if (ImGui::Button("Fly"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		c.fly = !c.fly;

	if (ImGui::Button("Wire"))     // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		if (!c.wire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		c.wire = !c.wire;
	}

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
