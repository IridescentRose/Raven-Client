#include "World.h"
#include <stddef.h>

namespace Minecraft::Internal {
	World::World()
	{
	}
	World::~World()
	{
	}
	void World::init()
	{
		is_loading = true;
		player = new Player();
	}
	void World::cleanup()
	{
	}
	void World::update()
	{
		if(!is_loading){
			player->update();
		}
	}
	void World::tickUpdate()
	{
	}
	void World::draw()
	{
		player->draw();
	}

	World* g_World = NULL;
}