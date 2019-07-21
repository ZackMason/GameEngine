/*****************************************************************
//	Author:				Zack Blaug
//	Date:				2/15/19
//	Description:		My First OpenGL Application
//
//
//
//
//
******************************************************************/

#pragma comment(lib, "./lib/SDL2.lib")
#pragma comment(lib, "./lib/glew32.lib")

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <cmath>
#include <map>
#include <stdlib.h>

#include "include/GLEW/GL/glew.h"
#include "include/SDL2/SDL.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl.h"
#include "imGUI/imgui_impl_opengl3.h"

#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureAttachment.h"
#include "Camera.h"
#include "Actor.h"
#include "Material.h"
#include "macros.h"
#include "FrameBuffer.h"
#include "Clock.h"
#include "Application.h"

//scene -> models --> shaders --> draw
//				  \_> texture _/

#undef main
int main(int argc, char* argv[])
{
	srand(time(NULL));
#if 0
	int mousex;
	int mousey;

	
	bool display_fps = true;

	int lastx = 400;
	int lasty = 400;
	int thisx = 400;
	int thisy = 400;

	//SDL_KeyboardEvent s_event;

	//used line ~123
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	Display display("title");

	glEnable(GL_PROGRAM_POINT_SIZE);

	//oh, look how far we've come
	Vertex screen_quad[] = { Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)),
						   Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
						   Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						   Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)),
						   Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						   Vertex(glm::vec3(1.0,1.0,0.0),glm::vec2(1.0,1.0))
	};

#if 0
	Vertex vertices1[] = { Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						  Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
						  Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)), };
#endif


	//Build and load mesh, needs more abstraction
	// better way to load info maybe

	Clock clock;

	std::vector<Actor> Actors;
	std::vector<Actor> Winds;

	std::vector<Actor> Water;
	std::vector<glm::vec2> Water_off;

	Actor skyActor("skydome2", "skydome", "skydome");
	//skyActor.m_transform->GetPos().y += 80.0f;
	skyActor.m_transform->GetScale() *= 80;

	Actor PlaneActor("biplane", "toon", "wood");
	PlaneActor.m_transform->GetPos().z -= 42;
	PlaneActor.m_transform->GetPos().x = 44;
	PlaneActor.m_transform->GetPos().y -= 3;
	Actors.push_back(PlaneActor);

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

			Water.push_back(world);
			Water_off.push_back(glm::vec2(x*20*water_scale,z*20*water_scale));
		}
	}
	water_scale = 16;
	water_size = 30;
	for (int x = -water_size; x <= water_size; x++)
	{
		for (int z = -water_size; z <= water_size; z++)
		{
			if (abs(z*20*water_scale) < 3 * 20 * 15 && abs(x*20*water_scale) < 3 * 20 * 15) continue;
			Actor world(mesh, water_mat);
			world.m_transform->GetPos().y -= 3.0f;
			world.m_transform->GetPos().z = z * 20.;
			world.m_transform->GetPos().x = x * 20.;
			world.m_transform->GetScale() *= water_scale;
			world.m_transform->GetScale().y = 1;

			Water.push_back(world);
			Water_off.push_back(glm::vec2(x * 20. * water_scale, z * 20. * water_scale));
		}
	}

	for (int i = 0; i < 0; i++)
	{
		std::cout << i << std::endl;
		std::string end = "";
		int pick = (rand() % 12) + 4;
		if (pick < 10) 
		{
			end = "0" + std::to_string(pick);
		}
		else
		{
			end = std::to_string(pick);
		}

		Actor tree1("TREES/tree_" + end, "tree", "tree_pallet", ".png");
		tree1.m_transform->GetPos().z = rand() % 400 - 200;
		tree1.m_transform->GetPos().x = rand() % 400 - 200;
		tree1.m_transform->GetRot().y = rand() % 360;
		tree1.m_transform->GetPos().y = -3.0f;
		tree1.m_transform->GetScale() *= 0.75f + ((sinf(rand()% 123124))*0.17);
		Actors.push_back(tree1);
	}

	for (int i = 0; i < 0; i++)
	{
		std::cout << i << std::endl;
		std::string end = "";
		int pick = (rand() % 3 + 1);
		if (pick < 10)
		{
			end = "0" + std::to_string(pick);
		}
		else
		{
			end = std::to_string(pick);
		}
		Actor tree1("MISC/plant_" + end, "standard", "tree_pallet", ".png");
		tree1.m_transform->GetPos  ().z = rand() % 400 - 200;
		tree1.m_transform->GetPos  ().x = rand() % 400 - 200;
		tree1.m_transform->GetRot  ().y = rand() % 180;
		tree1.m_transform->GetPos  ().y = -3.0f;
		tree1.m_transform->GetScale()  *= 3.0f;
		Actors.push_back(tree1);
	}

	std::shared_ptr<Mesh> wind_mesh = std::make_shared<Mesh>("./res/MESHS/wind_01.obj");
	std::shared_ptr<Texture> wind_tex = std::make_shared<Texture>("./res/TEXTURES/rain_grey.png");
	std::shared_ptr<Shader> wind_sdr = std::make_shared<Shader>("./res/SHADERS/windShader");
	std::shared_ptr<Material> wind_mat = std::make_shared<Material>(wind_sdr, wind_tex);
	for (int i = 40; i >= 0; i--)
	{
		Actor wind(wind_mesh, wind_mat);
		wind.m_transform->GetScale() *= 3.0f;
		Winds.push_back(wind);
	}


	Camera camera(glm::vec3(0.1, -145, 70), 70.0f, display.GetAspect(),1.f, 12000.0f);
	//Camera altCamera(glm::vec3(0, 0, 0), 70.0f, display.GetAspect(), 0.01f, 2000.0f);

	std::map<float, Actor*> sorted;
	

	float counter = 0.0f;

	bool auto_move = true;

	Transform cam_tran;
	float dv = 0.0f;

	FrameBuffer mfbo(true);

	Shader screen_shader("./res/SHADERS/outlineShader");
	//screen_shader.SetInt("diffuse", 0);
	//if (screen_shader.setFloat("Time", time_passed) == -1)
		//std::cout << "FAIL\n";

	Mesh screen(screen_quad, 6);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
/*
*******************************************************************************************
*******************************************************************************************
*******************************************************************************************
*******************************************************************************************
*/

	//std::cout << "mouse grab: " << SDL_SetRelativeMouseMode(SDL_TRUE) << " : " << SDL_CaptureMouse(SDL_TRUE) << std::endl;
	while (!display.IsClosed())
	{

		clock.Update();

		cam_tran.SetPos(camera.GetPos());


		//INPUT BEGIN HERE, NEEDS ABSTRACTION

		SDL_PumpEvents();

		//mouse recording

		(SDL_GetMouseState(&mousex, &mousey));
		if (SDL_GetRelativeMouseMode()) 
		{
			lastx = thisx;
			lasty = thisy;
			thisx = mousex;
			thisy = mousey;
			camera.RotY(-(lastx - thisx) /6.0f * clock.Get_DT());
			camera.RotX(-(lasty - thisy) /6.0f * clock.Get_DT());
		}

		//capture mouse when you click on the window
		if (SDL_GetMouseState(&mousex, &mousey))
		{
			SDL_CaptureMouse(SDL_TRUE);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		if (auto_move)
			camera.MoveForward(21.f);

		// release the mouse
		if (state[SDL_SCANCODE_E])
			auto_move = !auto_move;
		if (state[SDL_SCANCODE_ESCAPE])
			SDL_SetRelativeMouseMode(SDL_FALSE);
		
		if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
			camera.MoveForward(29.f);
		if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
			camera.MoveForward(-14.f);
		if (state[SDL_SCANCODE_I])
			camera.RotX(0.16f * clock.Get_DT());
		if (state[SDL_SCANCODE_K])
			camera.RotX(-0.16f * clock.Get_DT());
		if (state[SDL_SCANCODE_SPACE])
			camera.MoveUp(-14.f);
		if (state[SDL_SCANCODE_A])
			camera.MoveRight(14.f);
		if (state[SDL_SCANCODE_D])
			camera.MoveRight(-14.f);
		if (state[SDL_SCANCODE_B])
			display.SetIsClosed(true);
		if (state[SDL_SCANCODE_V])
			display_fps = !display_fps;
		if (state[SDL_SCANCODE_C])
			camera.MoveUp(14.f);
		if (state[SDL_SCANCODE_F]) 
		{
			SDL_SetWindowFullscreen(display.GetWindow(), SDL_WINDOW_FULLSCREEN);
			//display.SetWindowSize(1920, 1080);
			//SDL_SetVideoMode(SDL_FULLSCREEN);
			SDL_Surface* screenSurface = SDL_GetWindowSurface(display.GetWindow());
			SDL_UpdateWindowSurface(display.GetWindow());
			camera.SetAspect(display.GetAspect());
		}

		if (state[SDL_SCANCODE_Y] && !Actors.empty())
			Actors.pop_back();
		
		if (state[SDL_SCANCODE_U] && !Actors.empty())
		{
			std::string end = "";
			int pick = (rand() % 12) + 4;
			pick = 16;
			if (pick < 10)
			{
				end = "0" + std::to_string(pick);
			}
			else
			{
				end = std::to_string(pick);
			}

			Actor tree1("TREES/tree_" + end, "tree", "tree", ".png");
			//Actor tree1("TREES/tree_" + end, "tree", "tree_pallet", ".png");
			tree1.m_transform->GetPos().z = rand() % 1400 - 600;
			tree1.m_transform->GetPos().x = rand() % 1400 - 600;
			tree1.m_transform->GetRot().y = rand() % 360;
			tree1.m_transform->GetPos().y = 35.0f;
			tree1.m_transform->GetScale() *= 0.75f + ((sinf(rand() % 123124))*0.17);
			Actors.push_back(tree1);
		}
		
		display.Clear(0.5f,0.5f,0.5f,1.0f);

		//camera.LookAt(PlaneActor.m_transform->GetPos());

		auto height_v = cam_tran.GetPos();

		if (camera.m_position.y > -h(glm::vec2(height_v.x, height_v.z)))
			camera.m_position.y = -h(glm::vec2(height_v.x, height_v.z));
		//std::cout << "-h = " << -h(glm::vec2(height_v.x, height_v.z)) << "\n";
		camera.Update(clock.Get_DT());

		skyActor.m_transform->SetPos(camera.GetPos());
		skyActor.m_transform->GetPos().z *= -1;
		skyActor.m_transform->GetPos().x *= -1;
		skyActor.m_transform->GetPos().y *= -1;

		PlaneActor.m_transform->SetPos(camera.GetPos());
		PlaneActor.m_transform->GetPos() *= -1;

		auto lll = glm::lookAtRH(PlaneActor.m_transform->GetPos(), PlaneActor.m_transform->GetPos() - camera.GetForward() *5.f, camera.m_up);
		//PlaneActor.m_transform->GetRot().x = camera.GetRot().x;
		//PlaneActor.m_transform->GetRot().y = -camera.GetRot().y;
		//PlaneActor.m_transform->GetRot().x = glm::conjugate(glm::quat_cast(lll)).x;
		PlaneActor.m_transform->GetRot().x += -glm::pitch(glm::quat_cast(lll)) - PlaneActor.m_transform->GetRot().x;
		PlaneActor.m_transform->GetRot().y += -glm::yaw(glm::quat_cast(lll)) - PlaneActor.m_transform->GetRot().y;
		PlaneActor.m_transform->GetRot().z += glm::roll(glm::quat_cast(lll)) - PlaneActor.m_transform->GetRot().z;

		PlaneActor.m_transform->GetPos() -= camera.GetForward() * 50.0f;
		PlaneActor.m_transform->GetPos().y -= 10.0f;

		
		
		if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_L])
		{
			dv += 0.03 * (2-abs(dv));
			camera.RotY(0.16f * clock.Get_DT());
		}
		if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_J])
		{
			dv -= 0.03 * (2-abs(dv));
			camera.RotY(-0.16f * clock.Get_DT());
		}
		PlaneActor.m_transform->GetRot().z += dv;
		PlaneActor.m_transform->QUpdate();

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
		mfbo.Bind();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, .1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif


		//glDepthMask(GL_FALSE);
		skyActor.Draw(camera, clock.Get_Time_Passed());
		//glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (auto& a : Actors)
		{
			glm::vec3 ray = a.m_transform->GetPos() - camera.GetPos();
			ray = glm::normalize(ray);
			ray *= -1.0;
			//if (glm::dot(ray, camera.GetForward()) > -0.2)
				a.Draw(camera, clock.Get_Time_Passed());
		}

		for (unsigned int i = 0; i < Water.size(); i++)
		{
			Water[i].m_transform->GetPos().z = -camera.GetPos().z + Water_off[i].y;
			Water[i].m_transform->GetPos().x = -camera.GetPos().x + Water_off[i].x;
		}

		

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
		for (auto &a : Water)
		{
			a.Draw(camera, clock.Get_Time_Passed());
		}

#endif

		for (auto& w : Winds)
		{
			auto wind_look = glm::lookAtRH(w.m_transform->GetPos(), w.m_transform->GetPos() + camera.GetForward() *-30.f, glm::vec3(0, -1, 0));
			w.m_transform->GetRot().x += -glm::pitch(glm::quat_cast(wind_look)) - w.m_transform->GetRot().x;
			w.m_transform->GetRot().y += -glm::yaw(glm::quat_cast(wind_look)) - w.m_transform->GetRot().y;
			w.m_transform->GetRot().z += glm::roll(glm::quat_cast(wind_look)) - w.m_transform->GetRot().z;
			w.m_transform->QUpdate();

			if (glm::length(-camera.GetPos() - w.m_transform->GetPos()) > 300.f)
			{
				auto wv = camera.GetForward() * -200.f * ((rand() % 1000) / 1000.f);
				wv += 30.f * camera.GetRight() * ((rand() % 1000) - 500.f) / 1000.f;
				wv += 30.f * camera.GetUp() * ((rand() % 1000) - 500.f) / 1000.f;
				w.m_transform->SetPos(-camera.GetPos() + wv);
			}
			//glDepthMask(GL_FALSE);
			w.Draw(camera, clock.Get_Time_Passed());
			//glDepthMask(GL_TRUE);
		}


#if 1
		mfbo.Unbind();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		screen_shader.Bind();
		screen_shader.setFloat("Time", clock.Get_Time_Passed());

		glBindVertexArray(screen.GetVAO());
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, mfbo.GetColor());
		glGenerateMipmap(GL_TEXTURE_2D);
		mfbo.Bind_DTA(1);
		//glActiveTexture(1);
		//glBindTexture(GL_TEXTURE_2D, mfbo.GetDepth());
		screen_shader.SetInt("diffuse", 0);
		screen_shader.SetInt("depth", 1);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);
#endif

		display.Update();


		counter += 0.01f;
	}
#endif
	Application *myapp = new Application;
	Display *display = new Display("game", 1920, 1080);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	Level *level = new Level(*display);
	myapp->SetDisplay(display);
	myapp->SetLevel(level);
	myapp->OnStart();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(1920, 1080);
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(display->GetWindow(), display->GetContext());
	ImGui_ImplOpenGL3_Init("#version 430");

	while (!myapp->GetDisplay().IsClosed())
	{

		
		myapp->OnUpdate();
	}
	delete myapp, display, level;
	return 0;
}