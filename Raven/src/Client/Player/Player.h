#pragma once
#include <Utilities/Timer.h>
#include <GFX/3D/Camera.h>

using namespace Stardust;

namespace Minecraft::Internal {
	class Player {
	public:

		Player();
		~Player();

		void update();
		void draw();

		inline GFX::Render3D::Camera* getCamera(){
			return cam;
		}

		double x, y, z;
		float yaw, pitch;
		bool onGround;
	private:
		GFX::Render3D::Camera* cam;
		bool positionChanged;
		bool rotationChanged;
		Utilities::Timer timer;
	};
}