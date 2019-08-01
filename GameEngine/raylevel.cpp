#include <iostream>
#include <string>
#include <filesystem>
#include "Application.h"
#include "RayLevel.h"
#include "Config.h"

static Vertex screen_quad[] = { Vertex(glm::vec3(-1.0, 1.0,0.0),glm::vec2(0.0,1.0)),
									  Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
									  Vertex(glm::vec3( 1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
									  Vertex(glm::vec3(-1.0, 1.0,0.0),glm::vec2(0.0,1.0)),
									  Vertex(glm::vec3( 1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
									  Vertex(glm::vec3( 1.0, 1.0,0.0),glm::vec2(1.0,1.0))  };

RayLevel::RayLevel(const Display &d) : m_camera(glm::vec3(0.1, -4, 0), 70.0f, d.GetAspect(), 1.f, 12000.0f),
m_ofbo(),
m_screen_sdr("./res/SHADERS/ray4Shader"),
m_screen(screen_quad, 6)
{
}


RayLevel::~RayLevel()
{
}

void RayLevel::Add_Actor(std::shared_ptr<Actor> actor)
{

}

void RayLevel::Display_Levels()
{
	namespace fs = std::experimental::filesystem;
	std::string path = "./res/LEVELS";
	for (const auto & entry : fs::directory_iterator(path))
		std::cout << entry.path() << "\n";

}

bool RayLevel::Load_Levels()
{

	return true;
}

void RayLevel::Render_Level()
{

}

void RayLevel::OnUpdate()
{
	auto  display = Application::Get().GetDisplay();
	auto  state = Application::Get().GetState();
	m_clock.Update();

	//INPUT BEGIN HERE, NEEDS ABSTRACTION

//SDL_PumpEvents();

	//mouse recording

	(SDL_GetMouseState(&mousex, &mousey));
	if (SDL_GetRelativeMouseMode())
	{
		lastx = thisx;
		lasty = thisy;
		thisx = mousex;
		thisy = mousey;
		m_camera.RotY(-(lastx - thisx) / 6.0f * m_clock.Get_DT());
		m_camera.RotX(-(lasty - thisy) / 6.0f * m_clock.Get_DT());
	}

#if 0
	//capture mouse when you click on the window
	if (SDL_GetMouseState(&mousex, &mousey))
	{
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
#endif

	// release the mouse
	if (state[SDL_SCANCODE_B])
		display->SetIsClosed(true);
	if (state[SDL_SCANCODE_ESCAPE])
		SDL_SetRelativeMouseMode(SDL_FALSE);
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
		m_camera.MoveForward(Config::Get().playerSpeed);
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
		m_camera.MoveForward(-14.f);
	if (state[SDL_SCANCODE_I])
		m_camera.RotX(0.46f * m_clock.Get_DT());
	if (state[SDL_SCANCODE_K])
		m_camera.RotX(-0.46f * m_clock.Get_DT());
	if (state[SDL_SCANCODE_SPACE])
		m_camera.MoveUp(-14.f);
	if (state[SDL_SCANCODE_A])
		m_camera.MoveRight(14.f);
	if (state[SDL_SCANCODE_D])
		m_camera.MoveRight(-14.f);
	if (state[SDL_SCANCODE_C])
		m_camera.MoveUp(14.f);
	if (state[SDL_SCANCODE_F])
	{
		SDL_SetWindowFullscreen(display->GetWindow(), SDL_WINDOW_FULLSCREEN);
		//display->SetWindowSize(1920, 1080);
		//SDL_SetVideoMode(SDL_FULLSCREEN);
		SDL_Surface* screenSurface = SDL_GetWindowSurface(display->GetWindow());
		SDL_UpdateWindowSurface(display->GetWindow());
		m_camera.SetAspect(display->GetAspect());
	}

	display->Clear(0.5f, 0.5f, 1.f, 1.0f);

	m_camera.Update(m_clock.Get_DT());


	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_L])
	{
		m_camera.RotY(0.816f * m_clock.Get_DT());
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_J])
	{
		m_camera.RotY(-0.816f * m_clock.Get_DT());
	}


	//Drawing begins here, needs abstraction

#if 1
	m_ofbo.Bind();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, .1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

#if 1
	m_ofbo.Unbind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_screen_sdr.Bind();
	glBindVertexArray(m_screen.GetVAO());
	m_ofbo.Bind_TA(0);
	glGenerateMipmap(GL_TEXTURE_2D);
	m_screen_sdr.setFloat("Time", m_clock.Get_Time_Passed());
	m_screen_sdr.setFloat("EPSILON", Config::Get().gamma);
	m_screen_sdr.setFloat("bias", Config::Get().bias);
	m_screen_sdr.setFloat("scale", Config::Get().scale);
	m_screen_sdr.setFloat("power", Config::Get().power);
	m_screen_sdr.setVec4("ucolor", Config::Get().shader_color);
	m_screen_sdr.setInt("diffuse", 0);
	m_screen_sdr.setInt("depth", 1);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#endif

}

void RayLevel::OnAttach()
{

}
