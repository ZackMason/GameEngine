#include <iostream>
#include <string>
#include <filesystem>

#include "Level.h"

Vertex screen_quad[] = { Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)),
						   Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
						   Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						   Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)),
						   Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						   Vertex(glm::vec3(1.0,1.0,0.0),glm::vec2(1.0,1.0))
};

Level::Level(const Display &d) : m_camera(glm::vec3(0.1, -145, 70), 70.0f, d.GetAspect(), 1.f, 12000.0f),
m_ofbo(true),
m_screen_sdr("./res/SHADERS/outlineShader"),
m_screen(screen_quad, 6),
m_player("biplane", "toon", "wood"),
m_sky("skydome2", "skydome", "skydome")
{
	m_sky.m_transform->GetScale() *= 80;
	m_player.m_transform->GetPos().z -= 42;
	m_player.m_transform->GetPos().x = 44;
	m_player.m_transform->GetPos().y -= 3;
}


Level::Level() : m_camera(glm::vec3(0.1, -145, 70), 70.0f, 1920./1080., 1.f, 12000.0f),
m_ofbo(true),
m_screen_sdr("./res/SHADERS/outlineShader"),
m_screen(screen_quad, 6),
m_sky("skydome2", "skydome", "skydome"),
m_player("biplane", "toon", "wood")
{

}

Level::~Level()
{
}

void Level::Add_Actor(std::shared_ptr<Actor> actor)
{

}

void Level::Display_Levels()
{
	namespace fs = std::experimental::filesystem;
	std::string path = "./res/LEVELS";
	for (const auto & entry : fs::directory_iterator(path))
		std::cout << entry.path() << "\n";

}

bool Level::Load_Levels()
{

	return true;
}

void Level::Render_Level()
{

}

void Level::OnUpdate(Display& display, const Uint8 * state)
{

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

	//capture mouse when you click on the window
	if (SDL_GetMouseState(&mousex, &mousey))
	{
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	if (auto_move)
		m_camera.MoveForward(51.f);

	// release the mouse
	if (state[SDL_SCANCODE_B])
		display.SetIsClosed(true);
	if (state[SDL_SCANCODE_E])
		auto_move = !auto_move;
	if (state[SDL_SCANCODE_ESCAPE])
		SDL_SetRelativeMouseMode(SDL_FALSE);
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
		m_camera.MoveForward(29.f);
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
		m_camera.MoveForward(-14.f);
	if (state[SDL_SCANCODE_I])
		m_camera.RotX(0.16f * m_clock.Get_DT());
	if (state[SDL_SCANCODE_K])
		m_camera.RotX(-0.16f * m_clock.Get_DT());
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
		SDL_SetWindowFullscreen(display.GetWindow(), SDL_WINDOW_FULLSCREEN);
		//display.SetWindowSize(1920, 1080);
		//SDL_SetVideoMode(SDL_FULLSCREEN);
		SDL_Surface* screenSurface = SDL_GetWindowSurface(display.GetWindow());
		SDL_UpdateWindowSurface(display.GetWindow());
		m_camera.SetAspect(display.GetAspect());
	}

	display.Clear(0.5f, 0.5f, 0.5f, 1.0f);

	//camera.LookAt(PlaneActor.m_transform->GetPos());

	//std::cout << "-h = " << -h(glm::vec2(height_v.x, height_v.z)) << "\n";
	m_camera.Update(m_clock.Get_DT());

	m_sky.m_transform->SetPos(m_camera.GetPos());
	m_sky.m_transform->GetPos().z *= -1;
	m_sky.m_transform->GetPos().x *= -1;
	m_sky.m_transform->GetPos().y *= -1;

	m_player.m_transform->SetPos(m_camera.GetPos());
	m_player.m_transform->GetPos() *= -1;

	auto lll = glm::lookAtRH(m_player.m_transform->GetPos(), m_player.m_transform->GetPos() - m_camera.GetForward() *5.f, m_camera.m_up);
	//PlaneActor.m_transform->GetRot().x = camera.GetRot().x;
	//PlaneActor.m_transform->GetRot().y = -camera.GetRot().y;
	//PlaneActor.m_transform->GetRot().x = glm::conjugate(glm::quat_cast(lll)).x;
	m_player.m_transform->GetRot().x += -glm::pitch(glm::quat_cast(lll)) - m_player.m_transform->GetRot().x;
	m_player.m_transform->GetRot().y += -glm::yaw(glm::quat_cast(lll)) - m_player.m_transform->GetRot().y;
	m_player.m_transform->GetRot().z += glm::roll(glm::quat_cast(lll)) - m_player.m_transform->GetRot().z;

	m_player.m_transform->GetPos() -= m_camera.GetForward() * 50.0f;
	m_player.m_transform->GetPos().y -= 10.0f;



	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_L])
	{
		dv += 0.03 * (2 - abs(dv));
		m_camera.RotY(0.16f * m_clock.Get_DT());
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_J])
	{
		dv -= 0.03 * (2 - abs(dv));
		m_camera.RotY(-0.16f * m_clock.Get_DT());
	}
	m_player.m_transform->GetRot().z += dv;
	m_player.m_transform->QUpdate();

	dv *= .94;
#if 0
	//TODO make hashtable for this kind of stuff
	//translate
	Actors[5].m_transform->GetPos().x += sinf(counter) / 100.0f;
	//rotate
	Actors[5].m_transform->GetRot().x = counter * 2;
	Actors[5].m_transform->GetRot().y = counter * 1;
	Actors[5].m_transform->GetRot().z = counter * 2;
#endif

	//Drawing begins here, needs abstraction

#if 1
	m_ofbo.Bind();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, .1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif


	//glDepthMask(GL_FALSE);
	m_player.Draw(m_camera, m_clock.Get_Time_Passed());
	//glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto& a : m_actors)
	{
		glm::vec3 ray = a.m_transform->GetPos() - m_camera.GetPos();
		ray = glm::normalize(ray);
		ray *= -1.0;
		//if (glm::dot(ray, camera.GetForward()) > -0.2)
		a.Draw(m_camera, m_clock.Get_Time_Passed());
	}

	for (unsigned int i = 0; i < m_terrain.size(); i++)
	{
		m_terrain[i].m_transform->GetPos().z = -m_camera.GetPos().z + Water_off[i].y;
		m_terrain[i].m_transform->GetPos().x = -m_camera.GetPos().x + Water_off[i].x;
	}

	m_player.Draw(m_camera, m_clock.Get_Time_Passed());

#if 0
	for (unsigned int i = 0; i < Water.size(); i++)
	{
		float distance = glm::length(camera.GetPos() - Water[i].m_transform->GetPos());
		sorted[distance] = &Water[i];
	}

	for (std::map<float, Actor*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		it->second->Draw(camera, time_passed);
	}
	sorted.clear();
#else
	for (auto &a : m_terrain)
	{
		a.Draw(m_camera, m_clock.Get_Time_Passed());
	}

#endif

#if 1
	for (auto& w : m_winds)
	{
		auto wind_look = glm::lookAtRH(w.m_transform->GetPos(), w.m_transform->GetPos() + m_camera.GetForward() *-30.f, glm::vec3(0, -1, 0));
		w.m_transform->GetRot().x += -glm::pitch(glm::quat_cast(wind_look)) - w.m_transform->GetRot().x;
		w.m_transform->GetRot().y += -glm::yaw(glm::quat_cast(wind_look)) - w.m_transform->GetRot().y;
		w.m_transform->GetRot().z += glm::roll(glm::quat_cast(wind_look)) - w.m_transform->GetRot().z;
		w.m_transform->QUpdate();

		if (glm::length(-m_camera.GetPos() - w.m_transform->GetPos()) > 300.f)
		{
			auto wv = m_camera.GetForward() * -200.f * ((rand() % 1000) / 1000.f);
			wv += 30.f * m_camera.GetRight() * ((rand() % 1000) - 500.f) / 1000.f;
			wv += 30.f * m_camera.GetUp() * ((rand() % 1000) - 500.f) / 1000.f;
			w.m_transform->SetPos(-m_camera.GetPos() + wv);
		}
		//glDepthMask(GL_FALSE);
		w.Draw(m_camera, m_clock.Get_Time_Passed());
		//glDepthMask(GL_TRUE);
	}
#endif


#if 1
	m_ofbo.Unbind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_screen_sdr.Bind();
	m_screen_sdr.setFloat("Time", m_clock.Get_Time_Passed());

	glBindVertexArray(m_screen.GetVAO());
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, m_ofbo.GetColor());
	glGenerateMipmap(GL_TEXTURE_2D);
	m_ofbo.Bind_DTA(1);
	//glActiveTexture(1);
	//glBindTexture(GL_TEXTURE_2D, mfbo.GetDepth());
	m_screen_sdr.SetInt("diffuse", 0);
	m_screen_sdr.SetInt("depth", 1);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#endif



	counter += 0.01f;
	
}

void Level::OnStart()
{
	int water_size = 3 * 5;
	int water_scale = 3;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("./res/MESHS/plane_02.obj");
	std::shared_ptr<Texture> water_tex = std::make_shared<Texture>("./res/TEXTURES/rock_grey.png");
	//std::shared_ptr<Shader> water_sdr = std::make_shared<Shader>("./res/SHADERS/terrianShader.2");
	std::shared_ptr<Shader> water_sdr = std::make_shared<Shader>("./res/SHADERS/terrianShader.3");
	std::shared_ptr<Material> water_mat = std::make_shared<Material>(water_sdr, water_tex);

	for (int x = -water_size; x <= water_size; x++)
	{
		for (int z = -water_size; z <= water_size; z++)
		{
			Actor world(mesh, water_mat);
			world.m_transform->GetPos().y -= 3.0f;
			world.m_transform->GetPos().z = z * 20.;
			world.m_transform->GetPos().x = x * 20.;
			world.m_transform->GetScale() *= water_scale;
			world.m_transform->GetScale().y = 1;

			m_terrain.push_back(world);
			Water_off.push_back(glm::vec2(x * 20 * water_scale, z * 20 * water_scale));
		}
	}
	water_scale = 12;
	water_size = 55;
	for (int x = -water_size; x <= water_size; x++)
	{
		for (int z = -water_size; z <= water_size; z++)
		{
			if (abs(z * 20 * water_scale) < 3 * 20 * 15 && abs(x * 20 * water_scale) < 3 * 20 * 15) continue;
			Actor world(mesh, water_mat);
			world.m_transform->GetPos().y -= 3.0f;
			world.m_transform->GetPos().z = z * 20.;
			world.m_transform->GetPos().x = x * 20.;
			world.m_transform->GetScale() *= water_scale;
			world.m_transform->GetScale().y = 1;

			m_terrain.push_back(world);
			Water_off.push_back(glm::vec2(x * 20. * water_scale, z * 20. * water_scale));
		}
	}
	std::shared_ptr<Mesh> wind_mesh = std::make_shared<Mesh>("./res/MESHS/wind_01.obj");
	std::shared_ptr<Texture> wind_tex = std::make_shared<Texture>("./res/TEXTURES/rain_grey.png");
	std::shared_ptr<Shader> wind_sdr = std::make_shared<Shader>("./res/SHADERS/windShader");
	std::shared_ptr<Material> wind_mat = std::make_shared<Material>(wind_sdr, wind_tex);
	for (int i = 30; i >= 0; i--)
	{
		Actor wind(wind_mesh, wind_mat);
		wind.m_transform->GetScale() *= 3.0f;
		m_winds.push_back(wind);
	}
}
