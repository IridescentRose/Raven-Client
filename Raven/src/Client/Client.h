#pragma once
#include <Network/NetworkDriver.h>
using namespace Stardust;

namespace Minecraft{
	class Client{
	public:
		Client();
		~Client();

		void init();
		void cleanup();
		void testPing();
		void update();
		void draw();
	private:
		Network::ClientSocket* csock;
	};
}