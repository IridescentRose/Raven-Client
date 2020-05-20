#include "Client.h"
#include <json/json.h>
#include <Network/NetworkDriver.h>
#include "Config.h"
#include <psprtc.h>

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
		getStatus();
		sceKernelDelayThread(1000 * 1000);
		//Then reconnect and handshake - login
		login();
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
			//utilityPrint("Lost connection!", LOGGER_LEVEL_WARN);
			//utilityPrint("Attempting Reconnect!", LOGGER_LEVEL_WARN);
			//if (!Network::g_NetworkDriver.Connect(g_Config.port, g_Config.ip.c_str(), false)) {
			//	throw std::runtime_error("Fatal: Could not connect to server!");
			//}
			//login();
		}
	}
	void Client::draw()
	{
		if (Network::g_NetworkDriver.m_Socket.isAlive()) {

		}
		else {

		}
	}


	//This is 1.12.2 protocol! Technically, we just need to swap protocol version to upgrade :)
	void Client::getStatus()
	{
		utilityPrint("Asking Server for Status!", LOGGER_LEVEL_INFO);

		Network::PacketOut* p = new Network::PacketOut();
		p->ID = 0x00;

		Network::encodeVarInt(340, p->bytes);
		Network::encodeStringNonNull(g_Config.ip, *p);
		Network::encodeShort(g_Config.port, *p);
		Network::encodeVarInt(CONNECTION_STATE_STATUS, p->bytes); //Status

		Network::g_NetworkDriver.AddPacket(p);
		Network::g_NetworkDriver.SendPackets(false);


		Network::PacketOut* p2 = new Network::PacketOut();
		p2->ID = 0x00;
		Network::g_NetworkDriver.AddPacket(p2);
		Network::g_NetworkDriver.SendPackets(false);

		//Wait for response
		Network::g_NetworkDriver.ReceivePacket(false);

		Network::PacketOut* p3 = new Network::PacketOut();
		p3->ID = 0x01;

		uint64_t time = 0;
		sceRtcGetCurrentTick(&time);
		Network::encodeLong(time, *p3);

		Network::g_NetworkDriver.AddPacket(p3);
		Network::g_NetworkDriver.SendPackets(false);

		//Wait for response
		Network::g_NetworkDriver.ReceivePacket(false);

		//We can safely assume we got both packets back!

		Network::PacketIn* p4 = Network::g_NetworkDriver.unhandledPackets.front();

		std::string jsonString = Network::decodeStringLE(*p4);

		Json::Value v;
		Json::Reader r;

		if (!r.parse(jsonString.c_str(), v)) {
			throw std::runtime_error("INVALID JSON!\n" + r.getFormattedErrorMessages());
		}

		//First check is protocol version!
		int prot = v["version"]["protocol"].asInt();
		if (prot != 340) {
			utilityPrint("Server is not a valid 1.12.2 server!", LOGGER_LEVEL_WARN);
		}

		//Display Server listing
		utilityPrint("Server #1 Status\n", LOGGER_LEVEL_INFO);
		utilityPrint("MOTD: " + v["description"]["text"].asString(), LOGGER_LEVEL_INFO);
		utilityPrint("Players: " + v["players"]["online"].asString() + "/" + v["players"]["max"].asString(), LOGGER_LEVEL_INFO);
		utilityPrint("Version: " + v["version"]["name"].asString() + "\n", LOGGER_LEVEL_INFO);

		//We should have 2 packets. We don't care about the pong.
		delete Network::g_NetworkDriver.unhandledPackets.front();
		Network::g_NetworkDriver.unhandledPackets.pop();
		delete Network::g_NetworkDriver.unhandledPackets.front();
		Network::g_NetworkDriver.unhandledPackets.pop();
	}

	void Client::login()
	{
		if (!Network::g_NetworkDriver.Connect(g_Config.port, g_Config.ip.c_str(), false)) {
			throw std::runtime_error("Fatal: Could not connect to server!");
		}

		utilityPrint("Logging into server!", LOGGER_LEVEL_INFO);

		Network::PacketOut* p = new Network::PacketOut();
		p->ID = 0x00;

		Network::encodeVarInt(340, p->bytes);
		Network::encodeStringNonNull(g_Config.ip, *p);
		Network::encodeShort(g_Config.port, *p);
		Network::encodeVarInt(CONNECTION_STATE_LOGIN, p->bytes); //Status

		Network::g_NetworkDriver.AddPacket(p);
		Network::g_NetworkDriver.SendPackets(false);

		Network::PacketOut* p2 = new Network::PacketOut();
		p2->ID = 0x00;

		Network::encodeStringNonNull(g_Config.username, *p2);
		Network::g_NetworkDriver.AddPacket(p2);
		Network::g_NetworkDriver.SendPackets(false);

		Network::g_NetworkDriver.ReceivePacket(false);

		Network::PacketIn* p3 = Network::g_NetworkDriver.unhandledPackets.front();

		//These do indeed work - are irrelevant for now
		std::string uuid = Network::decodeStringNonNullLE(*p3);
		std::string user = Network::decodeStringNonNullLE(*p3);

		delete Network::g_NetworkDriver.unhandledPackets.front();
		Network::g_NetworkDriver.unhandledPackets.pop();


	}

	Client* g_Client = NULL;
}