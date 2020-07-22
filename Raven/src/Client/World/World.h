#pragma once
#include <cstdint>

namespace Minecraft::Internal{

	struct TimeDataStruct {
		uint64_t age;
		uint64_t timeOfDay;
	};

	class World{
	public:

		World();
		~World();

		void init();
		void cleanup();

		void update();
		void tickUpdate();
		void draw();

		TimeDataStruct timeData;

	private:
	};

	extern World* g_World;
}