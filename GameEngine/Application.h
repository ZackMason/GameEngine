#pragma once

#include "Level.h"
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Actor.h"
#include "FrameBuffer.h"

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

private:
	Display* m_display;
	Level* m_level;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
};

