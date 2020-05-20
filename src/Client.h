#pragma once

#include "Utils.h"

namespace Minecraft::Client {
	class Client {
	public:
		Client();
		~Client();

		void run();

		bool isRunning();

		void update();
		void draw();
	private:
		bool running;
	};

	extern Client* g_Client;
}