#include "World.h"
#include <stddef.h>
#include <GFX/RenderCore.h>
#include <iostream>

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
		sky = new Rendering::Sky();
		timer.reset();
		timeData.age = 0;
		timeData.timeOfDay = 0;
		chunkMap.clear();
	}
	void World::cleanup()
	{
		if (chunkMap.size() > 0) {
			for (auto& [v, c] : chunkMap) {
				delete c;
			}
			chunkMap.clear();
		}
		chunkMap.clear();
	}
	void World::update()
	{
		if (!is_loading) {
			timer.deltaTime();

			if(timer.elapsed() > 0.050f){
				timer.reset();
				tickUpdate();
			}
			player->update();
		}
	}
	void World::tickUpdate()
	{
		timeData.age++;
		timeData.timeOfDay++;
		sky->Update(timeData.timeOfDay);
	}
	void World::draw()
	{
		GFX::g_RenderCore->set3DMode();
		sky->Draw(player->getCamera());

		GFX::g_RenderCore->setDefault2DMode();
		player->draw();
	}

	World* g_World = NULL;
}