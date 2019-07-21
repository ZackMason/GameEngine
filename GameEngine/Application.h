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


class Application
{
public:
	Application();
	~Application();

	void SetDisplay(Display* d) { m_display = d; }
	void SetLevel(Level* l) { m_level = l; }

	void OnUpdate();
	void OnStart();

	const Display& GetDisplay() { return *m_display; }

	inline Application& Get()
	{
		return *s_Instance;
	}

private:
	Display* m_display;
	Level* m_level;
	static Application* s_Instance;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
};

