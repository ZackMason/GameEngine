#pragma once
#include "Layer.h"

#include <vector>

#include "Actor.h"
#include "Camera.h"
#include "Clock.h"
#include "FrameBuffer.h"
#include "GBuffer.h"
#include "Display.h"
#include "Layer.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"


class Level2 : public Layer
{
public:
	Level2();
	Level2(const Display & d);
	~Level2();

	void Add_Actor(std::shared_ptr<Actor>);
	void Display_Levels();
	bool Load_Levels();
	void Render_Level();

	void OnUpdate() override;
	void OnAttach() override;

private:
	std::vector<Actor>	m_actors;
	std::vector<Actor>	m_terrain;
	std::vector<Actor>	m_winds;
	Actor					m_sky;
	Actor					m_player;
	Camera					m_camera;
	FrameBuffer				m_ofbo;  //off screen frame buffer
	GBuffer					m_gbo;  //off screen frame buffer
	Mesh					m_screen;
	Shader					m_screen_sdr;
	Shader					*scr_sdr2 = new Shader("./res/SHADERS/screenShader");

	std::vector<Light*>		m_lights;
	std::vector<glm::vec2> Water_off;
	Clock					m_clock;
	bool auto_move = true;
	float dv = 0, counter = 0;
	int mousex, mousey, lastx, thisx, lasty, thisy;
};

