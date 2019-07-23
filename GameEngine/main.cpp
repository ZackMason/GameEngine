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
	srand(time(NULL));

	Application *myapp = new Application;
	Display *display = new Display("game");
	myapp->SetDisplay(display);
	Layer *level = new Level(*display);
	Config *config = new Config();
	imGUILayer * gui = new imGUILayer();
	gui->SetName("Trip");
	myapp->AddLayer(level);
	myapp->AddOverlay(gui);
	//myapp->OnStart();

	while (!myapp->GetDisplay()->IsClosed())
	{	
		myapp->OnUpdate();
	}
	delete myapp, display, level, gui, config;
	return 0;
}