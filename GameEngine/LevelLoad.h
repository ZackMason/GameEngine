#pragma once
#include <iostream>
#include <string>
#include "Actor.h"

class LevelLoad
{
public:
	LevelLoad();
	virtual ~LevelLoad();

	int LoadFile();
	int PopulateScene(std::shared_ptr<Actor> Actors);

private:
	std::string file_name;

};

