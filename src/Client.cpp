#include "Client.h"
#include <Network/NetworkDriver.h>
#include "Config.h"

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
		parseServerConfig();
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

		if (!Network::g_NetworkDriver.Connect(g_Config.port, g_Config.ip.c_str(), false)) {
			throw std::runtime_error("Fatal: Could not connect to server!");
		}

		connState = CONNECTION_STATE_HANDSHAKE;
		//We need to send a handshake - get status
		//Then reconnect and handshake - login
	}
	bool Client::isRunning()
	{
		return running;
	}
	void Client::update()
	{
		if (Network::g_NetworkDriver.m_Socket.isAlive()) {
			//Do stuff
		}
		else {
			//If it's not alive - we have options
		}
	}
	void Client::draw()
	{
		if (Network::g_NetworkDriver.m_Socket.isAlive()) {

		}
		else {

		}
	}

	Client* g_Client = NULL;
}