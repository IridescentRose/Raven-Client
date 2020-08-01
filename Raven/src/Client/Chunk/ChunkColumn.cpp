#include "ChunkColumn.h"
#include <iostream>

namespace Minecraft::Internal::Chunks {
	ChunkColumn::ChunkColumn(int x, int z)
	{
		cX = x;
		cZ = z;

		sections.clear();

		for (int xx = 0; xx < CHUNK_SECTION_LENGTH; xx++) {
			for (int yy = 0; yy < CHUNK_SECTION_LENGTH; yy++) {
				biomes[xx][yy] = 1;
			}
		}
	}

	ChunkColumn::~ChunkColumn()
	{
		for (auto chnk : sections) {
			delete chnk;
		}
	}

	ChunkSection* ChunkColumn::getSection(uint8_t y)
	{
		for (auto chnk : sections) {
			if (chnk->getY() == y) {
				return chnk;
			}
		}
		return NULL;
	}

	void ChunkColumn::addSection(ChunkSection* chnks)
	{
		chnks->cX = cX;
		chnks->cZ = cZ;

		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++){
					std::cout << (int)chnks->blocks[(((y * 16) + z) * 16) + x] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			std::cout << y << std::endl;
		}

		if (chnks->isEmpty()) {
			delete chnks;
		}
		else {
			sections.push_back(chnks);
		}

	}

	void ChunkColumn::clearSections()
	{
		for (auto chnk : sections) {
			delete chnk;
		}
		sections.clear();
	}

}