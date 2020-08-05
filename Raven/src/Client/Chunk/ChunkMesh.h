#pragma once

namespace Minecraft::Internal::Chunks {
	class ChunkMesh {
	public:
		ChunkMesh();
		~ChunkMesh();

		void draw();
		void update();

	};
}