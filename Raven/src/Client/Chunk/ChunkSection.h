#pragma once
#include <map>
#include <mclib/common/Vector.h>
#include "ChunkDefines.h"
#include "ChunkMesh.h"

namespace Minecraft::Internal::Chunks {
	class ChunkMesh;

	class ChunkSection {
	public:
		ChunkSection(int y);
		~ChunkSection();

		bool isEmpty();

		BlockID getBlockAt(uint8_t x, uint8_t y, uint8_t z);
		uint8_t getLightingAt(uint8_t x, uint8_t y, uint8_t z);
		uint8_t getSkyLightAt(uint8_t x, uint8_t y, uint8_t z);

		void generateAnalytics();

		void draw();

		inline int getY() {
			return cY;
		}

		BlockID* blocks;
		uint8_t* sky_light;
		uint8_t* blk_light;
		int cX, cZ;
		bool empty;
		ChunkMesh* mesh;

		bool layerFull[16];
		bool isFilled;

		bool analytics;

	private:
		int cY;


	};
}