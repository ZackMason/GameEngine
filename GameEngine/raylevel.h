#pragma once
#include "Layer.h"

#include <vector>

#include "Actor.h"
#include "Camera.h"
#include "Clock.h"
#include "FrameBuffer.h"
#include "Display.h"
#include "Layer.h"


class RayLevel : public Layer
{
public:
	RayLevel();
	RayLevel(const Display & d);
	~RayLevel();

	void Add_Actor(std::shared_ptr<Actor>);
	void Display_Levels();
	bool Load_Levels();
	void Render_Level();

	void OnUpdate() override;
	void OnAttach() override;

private:
	std::vector<Actor>	m_actors;
	Camera					m_camera;
	FrameBuffer				m_ofbo;  //off screen frame buffer
	Mesh					m_screen;
	Shader					m_screen_sdr;
	Clock					m_clock;
	float dv = 0, counter = 0;
	int mousex, mousey, lastx, thisx, lasty, thisy;
};

