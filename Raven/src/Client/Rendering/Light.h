#pragma once

#include <Platform/Platform.h>
#include <GFX/GFXWrapper.h>
#include <GFX/3D/Camera.h>

namespace Minecraft::Rendering{
	using namespace Stardust;

	class SkyLight {
	public:
		SkyLight(unsigned int tex);
		~SkyLight();

		void Update(uint64_t timeOfDay);
		void Draw(GFX::Render3D::Camera* cam);

	private:
		float degrees;
		GFX::Model face;
		GFX::Mesh mesh;
		unsigned int texture;
	};
}