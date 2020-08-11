#include "ChunkSection.h"
#include <malloc.h>
#include <fstream>
#include <Utilities/Logger.h>
#include <iostream>

using namespace Stardust;

namespace Minecraft::Internal::Chunks {
	ChunkSection::ChunkSection(int y)
	{
		empty = true;
		cY = y;

		cX = 0;
		analytics = false;
		cZ = 0;

		blocks = (BlockID*)calloc(8192, 1);

		sky_light = (uint8_t*)calloc(2048, 1);
		mesh = NULL;
		blk_light = (uint8_t*)calloc(2048, 1);
	}

	ChunkSection::~ChunkSection()
	{
		free(blocks);
	}

	bool ChunkSection::isEmpty()
	{
		return empty;
	}

	BlockID ChunkSection::getBlockAt(uint8_t x, uint8_t y, uint8_t z)
	{
		return blocks[(((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x];
	}

	uint8_t ChunkSection::getLightingAt(uint8_t x, uint8_t y, uint8_t z)
	{
		int idx = ((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x);
		uint8_t lit = blk_light[idx / 2];

		if(idx % 2 == 0){
			return lit & 0x0F; //Low bits
		}else{
			return lit >> 4; //High bits
		}
	}

	uint8_t ChunkSection::getSkyLightAt(uint8_t x, uint8_t y, uint8_t z)
	{
		int idx = ((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x);
		uint8_t lit = sky_light[idx / 2];

		if (idx % 2 == 0) {
			return lit & 0x0F; //Low bits
		}
		else {
			return lit >> 4; //High bits
		}
	}
	void ChunkSection::generateAnalytics()
	{
		analytics = true;
		bool isFilled = true;
		for (int y = 0; y < 16; y++) {
			bool isFull = true;

			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					int idx = ((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x);
					if (blocks[idx] == 0) {
						isFull = false;
					}
				}
			}

			layerFull[y] = isFull;
			if (!isFull) {
				isFilled = false;
			}
		}
	}
	void ChunkSection::draw()
	{
		GFX::clearModelMatrix();
		GFX::translateModelMatrix({ cX * 16, cY * 16, cZ * 16 });

		if(mesh != NULL && mesh != nullptr)
			mesh->draw();

		GFX::clearModelMatrix();
	}
}
