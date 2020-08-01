#pragma once
#include <cstdint>
#include "../Player/Player.h"
#include "../Rendering/Sky.h"
#include <Utilities/Timer.h>
#include "../Chunk/ChunkColumn.h"


namespace Minecraft::Internal{

	using namespace Minecraft::Internal::Chunks;
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
		bool is_loading;
		Player* player;
		Rendering::Sky* sky;
		Utilities::Timer timer;
	private:
		std::map<mc::Vector3i, ChunkColumn*> chunkMap;

	};

	extern World* g_World;
}