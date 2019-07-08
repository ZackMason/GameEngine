#include <iostream>
#include <string>
#include <filesystem>

#include "Level.h"


Level::Level()
{
}


Level::~Level()
{
}

void Level::Add_Actor(std::shared_ptr<Actor> actor)
{

}

void Level::Display_Levels()
{
	namespace fs = std::experimental::filesystem;
	std::string path = "./res/LEVELS";
	for (const auto & entry : fs::directory_iterator(path))
		std::cout << entry.path() << "\n";

}

bool Level::Load_Levels()
{

	return true;
}

void Level::Render_Level()
{

}