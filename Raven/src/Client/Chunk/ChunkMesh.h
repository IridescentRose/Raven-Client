#pragma once
#include "ChunkSection.h"
#include <GFX/GFXWrapper.h>

namespace Minecraft::Internal::Chunks {
	using namespace Stardust::GFX;
	class ChunkSection;

	enum FacingType {
		FACE_XN,
		FACE_XP,
		FACE_ZN,
		FACE_ZP,
		FACE_YN,
		FACE_YP
	};

	class ChunkMesh {
	public:
		ChunkMesh();
		~ChunkMesh();

		void generate(ChunkSection* chks, ChunkSection* above, ChunkSection* below, ChunkSection* xn, ChunkSection* xp, ChunkSection* zn, ChunkSection* zp);

		void tryAddFace(const float blockFacePos[12], std::array<float, 8> texCoords, uint16_t facing, int type, glm::vec3 pos);

		void draw();

	private:
		bool genned;
		int numFaces;
		Mesh mesh;
		Model chunk;
	};
}