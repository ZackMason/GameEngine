#pragma once

#include "Level.h"
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
		if (m_display)
			delete m_display;
	};

	Application(const Application&  other) = delete;
	Application(const Application&& other) = delete;
	Application& operator=(const Application&  other) = delete;
	Application& operator=(const Application&& other) = delete;

	void SetDisplay(Display* d) { m_display = d; }
	void AddLayer(Layer* l)		{ m_LayerStack.PushLayer  (l); }
	void PopLayer(Layer* l)		{ m_LayerStack.PopLayer	  (l); }
	void AddOverlay(Layer* l)	{ m_LayerStack.PushOverlay(l); }
	void PopOverlay(Layer* l)	{ m_LayerStack.PopOverlay (l); }

	void OnUpdate();
	void OnStart();
	void OnExit();

	Display*     GetDisplay() { return m_display; }
	const Uint8* GetState()   { return state; }

	inline static Application& Get()
	{
		return *s_Instance;
	}

private:
	static Application* s_Instance;
	Display*			m_display;
	LayerStack			m_LayerStack;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
};

