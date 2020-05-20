#include "Client.h"
#include <Network/NetworkDriver.h>

namespace Minecraft::Client {
	Client::Client()
	{
		running = false;
	}
	Client::~Client()
	{
	}
	void Client::run()
	{
		running = true;
		utilityPrint("Starting Client...", LOGGER_LEVEL_INFO);
		if (!Network::g_NetworkDriver.Init()) {
			throw std::runtime_error("Fatal: Could not connect to Network! Check Stardust Core Logs!");
		}

#ifdef RAVEN_CLIENT_DEBUG
		pspDebugScreenInit();
		pspDebugScreenClear();
		pspDebugScreenSetXY(0, 0);
#endif

		if (!Network::g_NetworkDriver.Connect(65535, "IP ADDRESS", true)) {
			throw std::runtime_error("Fatal: Could not connect to server!");
		}

	}
	bool Client::isRunning()
	{
		return running;
	}
	void Client::update()
	{
	}
	void Client::draw()
	{
	}

	Client* g_Client = NULL;
}