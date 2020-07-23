#pragma once
#include <Platform/Platform.h>
#include <GFX/GFXWrapper.h>
#include <GFX/3D/Camera.h>
using namespace Stardust;
namespace Minecraft::Rendering{
	class Sky {
	public:
		Sky();
		~Sky();

		void Update(uint64_t timeOfDay);
		void Draw(GFX::Render3D::Camera* cam);

	private:
		GFX::Mesh face[6];
		GFX::Model faces[6];
	};
}