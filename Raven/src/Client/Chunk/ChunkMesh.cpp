#include "ChunkMesh.h"

namespace Minecraft::Internal::Chunks {

	const float backFace[12] = {
		0, 0, 1, 
		1, 0, 1, 
		1, 1, 1, 
		0, 1, 1,
	};
	
	const float frontFace[12] = {
		1, 0, 0,
		0, 0, 0, 
		0, 1, 0, 
		1, 1, 0,
	};

	const float leftFace[12] = {
		0, 0, 0, 
		0, 0, 1, 
		0, 1, 1, 
		0, 1, 0,
	};
	
	const float rightFace[12] = {
		1, 0, 1,
		1, 0, 0,
		1, 1, 0, 
		1, 1, 1,
	};

	const float topFace[12] = {
		0, 1, 0,
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,
	};

	const float bottomFace[12] = {
		0, 0, 0,
		1, 0, 0,
		1, 0, 1,
		0, 0, 1,
	};

	ChunkMesh::ChunkMesh() {
		numFaces = 0;
		genned = false;
	}

	ChunkMesh::~ChunkMesh(){

	}

	void ChunkMesh::generate(ChunkSection* chks, ChunkSection* above, ChunkSection* below, ChunkSection* xn, ChunkSection* xp, ChunkSection* zn, ChunkSection* zp)
	{
		if(!chks->isFilled){
			for (int y = 0; y < 16; y++) {
				for (int z = 0; z < 16; z++) {
					for (int x = 0; x < 16; x++) {
						int idx = (((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x;

						uint16_t id = chks->blocks[idx];
						glm::vec3 pos = { x, y, z };

						//No air mesh gen
						if (id == 0) {
							continue;
						}

						
						std::array<float, 8> coords = {
							0, 0,
							1, 0,
							1, 1,
							0, 1
						};

						//Try adding stuff here!
						
						int xnb = x - 1;
						uint16_t xnid = 0;
						if (xnb < 0 && xn != NULL) {
							xnid = xn->blocks[((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + 15)];
						}
						else if(xnb >= 0){
							xnid = chks->blocks[((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + (x - 1))];
						}
						int xpb = x + 1;
						uint16_t xpid = 0;
						if (xpb > 15 && xp != NULL) {
							xpid = xp->blocks[((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + 0)];
						}
						else if(xpb < 16){
							xpid = chks->blocks[((((y * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + (x + 1))];
						}

						int znb = z - 1;
						uint16_t znid = 0;
						if (znb < 0 && zn != NULL) {
							znid = zn->blocks[((((y * CHUNK_SECTION_LENGTH) + 15) * CHUNK_SECTION_LENGTH) + x)];
						}
						else if(znb >= 0){
							znid = chks->blocks[((((y * CHUNK_SECTION_LENGTH) + (z - 1)) * CHUNK_SECTION_LENGTH) + x)];
						}
						int zpb = z + 1;
						uint16_t zpid = 0;
						if (zpb > 15 && zp != NULL) {
							zpid = zp->blocks[((((y * CHUNK_SECTION_LENGTH) + 0) * CHUNK_SECTION_LENGTH) + 0)];
						}
						else if(zpb < 16){
							zpid = chks->blocks[((((y * CHUNK_SECTION_LENGTH) + (z+1)) * CHUNK_SECTION_LENGTH) + x)];
						}

						int ynb = y - 1;
						uint16_t ynid = 0;
						if (ynb < 0 && below != NULL) {
							ynid = below->blocks[((((15 * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x)];
						}
						else if(ynb >= 0) {
							ynid = chks->blocks[(((( (y-1) * CHUNK_SECTION_LENGTH) + z) * CHUNK_SECTION_LENGTH) + x)];
						}
						int ypb = y + 1;
						uint16_t ypid = 0;
						if (ypb > 15 && above != NULL) {
							ypid = above->blocks[((((0 * CHUNK_SECTION_LENGTH) + (z + 1)) * CHUNK_SECTION_LENGTH) + 0)];
						}
						else if (ypb < 16){
							ypid = chks->blocks[(((( (y+1) * CHUNK_SECTION_LENGTH) + (z + 1)) * CHUNK_SECTION_LENGTH) + (x + 1))];
						}

						tryAddFace(leftFace, coords, xnid, FACE_XN, pos);
						tryAddFace(rightFace, coords, xpid, FACE_XP, pos);
						tryAddFace(frontFace, coords, znid, FACE_ZN, pos);
						tryAddFace(backFace, coords, zpid, FACE_ZP, pos);
						tryAddFace(bottomFace, coords, ynid, FACE_YN, pos);
						tryAddFace(topFace, coords, ypid, FACE_YP, pos);

					}
				}
			}

			chunk.addData(mesh);
			std::cout << "CHONK GEN with FACES: " << numFaces << std::endl;
		}

		genned = true;
	}

	void ChunkMesh::tryAddFace(const float blockFacePos[12], std::array<float, 8> texCoords, uint16_t facing, int type, glm::vec3 pos)
	{
		if (facing == 0) {
			numFaces++;

			mesh.position.push_back(blockFacePos[0] + pos.x);
			mesh.position.push_back(blockFacePos[1] + pos.y);
			mesh.position.push_back(blockFacePos[2] + pos.z);
			mesh.uv.push_back(texCoords[0]);
			mesh.uv.push_back(texCoords[1]);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);

			mesh.position.push_back(blockFacePos[3] + pos.x);
			mesh.position.push_back(blockFacePos[4] + pos.y);
			mesh.position.push_back(blockFacePos[5] + pos.z);
			mesh.uv.push_back(texCoords[2]);
			mesh.uv.push_back(texCoords[3]);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);

			mesh.position.push_back(blockFacePos[6] + pos.x);
			mesh.position.push_back(blockFacePos[7] + pos.y);
			mesh.position.push_back(blockFacePos[8] + pos.z);
			mesh.uv.push_back(texCoords[4]);
			mesh.uv.push_back(texCoords[5]);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);

			mesh.position.push_back(blockFacePos[9] + pos.x);
			mesh.position.push_back(blockFacePos[10] + pos.y);
			mesh.position.push_back(blockFacePos[11] + pos.z);
			mesh.uv.push_back(texCoords[6]);
			mesh.uv.push_back(texCoords[7]);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);
			mesh.color.push_back(1.0f);

			mesh.indices.push_back(0 + numFaces * 4);
			mesh.indices.push_back(1 + numFaces * 4);
			mesh.indices.push_back(2 + numFaces * 4);
			mesh.indices.push_back(2 + numFaces * 4);
			mesh.indices.push_back(3 + numFaces * 4);
			mesh.indices.push_back(0 + numFaces * 4);

		}
	}

	void ChunkMesh::draw(){
		if(genned)
			chunk.draw();
	}
}