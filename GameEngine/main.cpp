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


#include <stdlib.h>

#include "include/GLEW/GL/glew.h"
#include "include/SDL2/SDL.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl.h"
#include "imGUI/imgui_impl_opengl3.h"

//scene -> models --> shaders --> draw
//				  \_> texture _/


#undef main
int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(NULL)));

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
	return 0;
}