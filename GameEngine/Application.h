#pragma once

#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Actor.h"
#include "FrameBuffer.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl.h"
#include "imGUI/imgui_impl_opengl3.h"
#include "LayerStack.h"

class Application
{
public:
	Application();
	~Application()
	{
	};

	Application(const Application&  other) = delete;
	Application(const Application&& other) = delete;
	Application& operator=(const Application&  other) = delete;
	Application& operator=(const Application&& other) = delete;

	void SetDisplay(std::shared_ptr<Display> d) { m_display = d; }

	void AddLayer(std::shared_ptr<Layer> l) { m_LayerStack.PushLayer(l); }
	void PopLayer(std::shared_ptr<Layer> l) { m_LayerStack.PopLayer(l); }
	void AddOverlay(std::shared_ptr<Layer> l)	{ m_LayerStack.PushOverlay(l); }
	void PopOverlay(std::shared_ptr<Layer> l)	{ m_LayerStack.PopOverlay (l); }

	void OnUpdate();
	void OnStart();
	void OnExit();

	std::shared_ptr<Display>     GetDisplay() { return m_display; }
	const Uint8* GetState()   { return state; }

	inline static Application& Get()
	{
		return *s_Instance;
	}
	std::vector<std::vector<std::string>> light_names;

private:
	static Application*			s_Instance;

private:
	std::shared_ptr<Display>	m_display;
	LayerStack					m_LayerStack;
	const Uint8 *				state =	SDL_GetKeyboardState(NULL);
};

