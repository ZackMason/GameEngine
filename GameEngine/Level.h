#pragma once

#include <vector>

#include "Actor.h"
#include "Camera.h"
#include "Clock.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Display.h"



class Level
{
public:
	Level();
	Level(const Display & d);
	~Level();

	void Add_Actor(std::shared_ptr<Actor>);
	void Display_Levels();
	bool Load_Levels();
	void Render_Level();

	void OnUpdate(Display& display, const Uint8*);
	void OnStart();

private:
	std::vector<Actor>	m_actors;
	std::vector<Actor>	m_terrain;
	std::vector<Actor>	m_winds;
	Actor					m_sky;
	Actor					m_player;
	Camera					m_camera;
	FrameBuffer				m_ofbo;  //off screen frame buffer
	Mesh					m_screen;
	Shader					m_screen_sdr;
	std::vector<glm::vec2> Water_off;
	Clock					m_clock;
	bool auto_move = true;
	float dv = 0, counter = 0;
	int mousex, mousey,lastx,thisx,lasty,thisy;
};

