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

#include "CORE.h"
#include "TestLevel.h"
#include "Level.h"

#include "EntityManager.h"
#include "TransformSystem.h"
#include "TransformComponent.h"
#include "./Physics/SphereCollider.h"

#include <stdlib.h>

#include "include/GLEW/GL/glew.h"
#include "include/SDL2/SDL.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl.h"
#include "imGUI/imgui_impl_opengl3.h"

//scene -> models --> shaders --> draw
//				  \_> texture _/

#define _LOG(x) {std::cout << x << "\n";}

#undef main
int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(NULL)));


	
	EntityManager manager;
	Entity e1(0xedfe);
	TransformSystem ts;
	TransformComponent* tc = new TransformComponent(e1.GetID());
	SphereCollider* sc = new SphereCollider(e1.GetID());
	TransformComponent* tc2 = new TransformComponent(e1.GetID());
	e1.AddComponent(sc->GetID(), sc);
	e1.AddComponent(tc->GetID(), tc);

	//auto t3 = TransformComponent::CREATE_FUNCTION(manager.m_components[tc->GetID()], e1.GetID(), tc);

	manager.AddEntity(&e1);
	manager.AddSystem(&ts);
	std::cout << tc->GetID() << "\n";
	std::cout << tc2->GetID() << "\n";
	std::cout << sc->GetID() << "\n";

	auto t = e1.GetComponent<TransformComponent>();
	auto c = e1.GetComponent<SphereCollider>();
	_LOG("x: " << t->m_position.x)
	_LOG("z:" << t->m_position.z)

#if 0

	std::shared_ptr<Application> myapp = std::make_shared<Application>();
	std::shared_ptr<Display> display = std::make_shared<Display>("game");
	myapp->SetDisplay(display);
	std::shared_ptr<Layer>   level  = std::make_shared< Level2 > (*display);
	std::shared_ptr<Config>  config = std::make_shared<Config>();
	std::shared_ptr<imGUILayer> gui = std::make_shared<imGUILayer>();
	
	gui->SetName("Trip");
	myapp->AddLayer(level);
	myapp->AddOverlay(gui);
	glEnable(GL_DEBUG_OUTPUT);
	myapp->OnStart();
	std::string test = "works";




	while (!myapp->GetDisplay()->IsClosed())
	{
		myapp->OnUpdate();
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 1, GL_DEBUG_SEVERITY_HIGH,200,"OPENGL ERROR: ");
	}
#endif
	return 0;
}