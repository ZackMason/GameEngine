#pragma once
#include "Actor.h"

class Level
{
public:
	Level();
	~Level();

	void Add_Actor(std::shared_ptr<Actor>);
	void Display_Levels();
	bool Load_Levels();
	void Render_Level();

private:
	std::shared_ptr<Actor> m_actors;
};

