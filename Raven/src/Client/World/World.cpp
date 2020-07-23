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
	}
	void World::cleanup()
	{
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
		timeData.timeOfDay = timeData.age % 24000;
		sky->Update(timeData.age);
		std::cout << timeData.age << std::endl;
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