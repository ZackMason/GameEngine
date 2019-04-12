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
#include <stdlib.h>

#include "include/GLEW/GL/glew.h"
#include "include/SDL2/SDL.h"

#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Actor.h"
#include "Material.h"
#include "macros.h"

//scene -> models --> shaders --> draw
//				  \_> texture _/

double UpdateTime()
{
	time_t timer;
	struct tm y2k = { 0 };

	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */

	return difftime(timer, mktime(&y2k));
}

#undef main
int main(int argc, char* argv[])
{
	srand(time(NULL));

	int mousex;
	int mousey;

	double time_passed = 0;
	double delta_time = 0;

	int lastx = 400;
	int lasty = 400;
	int thisx = 400;
	int thisy = 400;

	double last_time  = 0.0;
	double start_time = UpdateTime();

	double fps = 0.0;
	int frames = 0;
	//SDL_KeyboardEvent s_event;

	//used line ~123
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	Display display("title");

	glEnable(GL_PROGRAM_POINT_SIZE);

	//oh, look how far we've come
#if 0
	Vertex vertices[] = { Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						  Vertex(glm::vec3(1.0,1.0,0.0),glm::vec2(1.0,1.0)),
						  Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)), };

	Vertex verticesT[] = { Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						  Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
						  Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)), };
#endif


	//Build and load mesh, needs more abstraction
	// better way to load info maybe

	std::vector<Actor> Actors;

	Actor skyActor("sky", "sky", "nsky");
	skyActor.m_transform->GetPos().y += 80.0f;
	skyActor.m_transform->GetScale() *= 130;

	Actor boxActor("cube", "mandel");
	boxActor.m_transform->GetPos().z -= 37;
	boxActor.m_transform->GetPos().z -= 37;
	boxActor.m_transform->GetScale() *= 2.0f;

	Actors.push_back(boxActor);
	Actors.push_back(skyActor);

	Actor box("monkey", "standard", "UVgrid");
	box.m_transform->GetPos().x = -6.0f;
	box.m_transform->GetPos().y -= 0.5f;

	Actor world("plane", "ground", "grass");
	world.m_transform->GetPos().y -= 3.0f;
	world.m_transform->GetScale() *= 200;
	Actors.push_back(world);

	Actor room("torus", "toon");
	room.m_transform->GetPos().z += 37;
	room.m_transform->GetPos().y -= 2;
	Actors.push_back(room);

	Actor gun("torus", "toon");
	gun.m_transform->GetPos().x = 230;

	Actor torus("monkey", "toon");
	torus.m_transform->GetPos().y = 3.0f;
	torus.m_transform->GetPos().x = 6.0f;
	torus.m_transform->GetScale() *= 0.5f;

	Actors.push_back(gun);
	Actors.push_back(torus);

	Actors.push_back(box);

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
		tree1.m_transform->GetScale() *= 3.0f;
		Actors.push_back(tree1);
	}

	Actor tree2("rcube", "toon");
	tree2.m_transform->GetPos().y = -2.0f;
	tree2.m_transform->GetPos().z = 4.0f;

	Actors.push_back(tree2);

	Camera camera(glm::vec3(0, 0, 7), 70.0f, display.GetAspect(),0.01f, 2000.0f);
	//Camera altCamera(glm::vec3(0, 0, 0), 70.0f, display.GetAspect(), 0.01f, 2000.0f);

	float counter = 0.0f;

	std::chrono::steady_clock::time_point begin, end;
	begin = std::chrono::steady_clock::now();


	//std::cout << "mouse grab: " << SDL_SetRelativeMouseMode(SDL_TRUE) << " : " << SDL_CaptureMouse(SDL_TRUE) << std::endl;
	while (!display.IsClosed())
	{
		frames++;
		
		//TIMER, needs abstraction

		end = std::chrono::steady_clock::now();

		last_time = UpdateTime();
		delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0f;
		time_passed += delta_time;

		begin = std::chrono::steady_clock::now();

		if (last_time - start_time > 6.25 && frames > 10)
		{
			fps = (double)frames / (last_time - start_time);
			start_time = last_time;
			//std::cout << time_passed << std::endl;
			frames = 0;
			//std::cout << "camera " << camera.GetForward().x << std::endl;
			std::cout << "FPS: " << fps << std::endl;
			//std::cout << "x: " << *mousex << ", y: " << *mousey << std::endl;
		}

		//INPUT BEGIN HERE, NEEDS ABSTRACTION

		SDL_PumpEvents();

		//mouse recording

		(SDL_GetMouseState(&mousex, &mousey));
		if (SDL_GetRelativeMouseMode()) {
			lastx = thisx;
			lasty = thisy;
			thisx = mousex;
			thisy = mousey;
			camera.RotY(-(lastx - thisx) /4.0 * delta_time);
			camera.RotX(-(lasty - thisy) /4.0 * delta_time);
		}

		//capture mouse when you click on the window
		if (SDL_GetMouseState(&mousex, &mousey))
		{
			SDL_CaptureMouse(SDL_TRUE);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		// release the mouse
		if (state[SDL_SCANCODE_ESCAPE])
			SDL_SetRelativeMouseMode(SDL_FALSE);
		if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_L])
			camera.RotY(0.056f);
		if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_J])
			camera.RotY(-0.056f);
		if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
			camera.MoveForward(0.03f);
		if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
			camera.MoveForward(-0.03f);
		if (state[SDL_SCANCODE_I])
			camera.RotX(0.056f);
		if (state[SDL_SCANCODE_K])
			camera.RotX(-0.056f);
		if (state[SDL_SCANCODE_SPACE])
			camera.MoveUp(-0.17f);
		if (state[SDL_SCANCODE_A])
			camera.MoveRight(0.03f);
		if (state[SDL_SCANCODE_D])
			camera.MoveRight(-0.03f);
		if (state[SDL_SCANCODE_B])
			display.SetIsClosed(true);
		if (state[SDL_SCANCODE_C])
			camera.m_position.y += 30.0 * delta_time;
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
			tree1.m_transform->GetScale() *= 0.75f + ((sinf(rand() % 123124))*0.17);
			Actors.push_back(tree1);
		}
		
		display.Clear(0.0f,0.0f,0.8f,1.0f);

		camera.Update();
		
		//TODO make hashtable for this kind of stuff
		//translate
		Actors[5].m_transform->GetPos().x += sinf(counter) / 100.0f;
		//rotate
		Actors[5].m_transform->GetRot().z = counter * 2;
		Actors[5].m_transform->GetRot().y = counter * 1;
		Actors[5].m_transform->GetRot().x = counter * 2;

		//Drawing begins here, needs abstraction

		for (auto& a : Actors)
		{
			a.Draw(camera, time_passed);
		}

		display.Update();

		counter += 0.01f;
	}

	return 0;
}