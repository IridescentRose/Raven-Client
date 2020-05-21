#pragma once

#include "Utils.h"
#include <Utilities/Thread.h>

namespace Minecraft::Client {

	enum ConnectionStates {
		CONNECTION_STATE_HANDSHAKE = 0x0,
		CONNECTION_STATE_STATUS = 0x1,
		CONNECTION_STATE_LOGIN = 0x2,
		CONNECTION_STATE_PLAY = 0x3,
	};

	class Client {
	public:
		Client();
		~Client();

		void run();

		bool isRunning();

		void update();
		void draw();

		int connState;

	private:

		void getStatus();

		void login();

		static int thread_network(unsigned int i, void* a);

		bool running;
		Thread* thr;
	};

	extern Client* g_Client;
}