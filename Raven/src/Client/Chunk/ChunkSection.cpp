#include "ChunkSection.h"
#include <malloc.h>
#include <fstream>
#include <Utilities/Logger.h>
#include <iostream>

namespace Minecraft::Internal::Chunks {
	ChunkSection::ChunkSection(int y)
	{
		empty = true;
		cY = y;

		cX = 0;
		cZ = 0;

		blocks = (BlockID*)malloc(8192);

		sky_light = (uint8_t*)malloc(2048);
		blk_light = (uint8_t*)malloc(2048);
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
}
