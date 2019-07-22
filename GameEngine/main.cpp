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
#include "imGUILayer.h"
#include "Config.h"
#include "level2.h"

//scene -> models --> shaders --> draw
//				  \_> texture _/

#undef main
int main(int argc, char* argv[])
{
	srand(time(NULL));

	Application *myapp = new Application;
	Display *display = new Display("game");
	Layer *level = new Level(*display);
	Config *config = new Config();
	imGUILayer * gui = new imGUILayer();
	gui->SetName("Trip");
	myapp->SetDisplay(display);
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