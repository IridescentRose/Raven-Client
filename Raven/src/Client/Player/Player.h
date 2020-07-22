#pragma once
#include <Utilities/Timer.h>

using namespace Stardust;

namespace Minecraft::Internal {
	class Player {
	public:

		Player();
		~Player();

		void update();
		void draw();

		double x, y, z;
		float yaw, pitch;
		bool onGround;
	private:
		bool positionChanged;
		bool rotationChanged;
		Utilities::Timer timer;
	};
}