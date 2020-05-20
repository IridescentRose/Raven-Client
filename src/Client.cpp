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


	//This is 1.12.2 protocol! Technically, we just need to swap protocol version to upgrade :)
	void Client::getStatus()
	{
		Network::PacketOut* p = new Network::PacketOut();
		p->ID = 0x00;

		Network::encodeVarInt(340, p->bytes);
		Network::encodeStringNonNull(g_Config.ip, *p);
		Network::encodeShort(g_Config.port, *p);
		Network::encodeVarInt(CONNECTION_STATE_STATUS, p->bytes); //Status

		Network::g_NetworkDriver.AddPacket(p);
		Network::g_NetworkDriver.SendPackets(false);

		utilityPrint("Sending Handshake for Status!", LOGGER_LEVEL_INFO);

		Network::PacketOut* p2 = new Network::PacketOut();
		p2->ID = 0x00;
		Network::g_NetworkDriver.AddPacket(p2);
		Network::g_NetworkDriver.SendPackets(false);
		utilityPrint("Sending Status Request", LOGGER_LEVEL_INFO);

		//Wait for response
		Network::g_NetworkDriver.ReceivePacket(false);

		Network::PacketOut* p3 = new Network::PacketOut();
		p3->ID = 0x01;

		uint64_t time = 0;
		sceRtcGetCurrentTick(&time);
		Network::encodeLong(time, *p3);

		Network::g_NetworkDriver.AddPacket(p3);
		Network::g_NetworkDriver.SendPackets(false);
		utilityPrint("Sending Ping!", LOGGER_LEVEL_INFO);

		//Wait for response
		Network::g_NetworkDriver.ReceivePacket(false);

		//We can safely assume we got both packets back!
		utilityPrint("Packets received... reading...", LOGGER_LEVEL_DEBUG);
		utilityPrint("Packet Count: " + std::to_string(Network::g_NetworkDriver.unhandledPackets.size()), LOGGER_LEVEL_TRACE);

		Network::PacketIn* p4 = Network::g_NetworkDriver.unhandledPackets.front();

		utilityPrint("First packet: " + std::to_string(p4->ID), LOGGER_LEVEL_DEBUG); //Alright, we have something good here :D

		std::string jsonString = Network::decodeStringLE(*p4);
		utilityPrint(jsonString, LOGGER_LEVEL_TRACE);

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
		utilityPrint("Server #1\n", LOGGER_LEVEL_INFO);
		utilityPrint("Description: " + v["description"]["text"].asString(), LOGGER_LEVEL_INFO);
		utilityPrint("Players: " + v["players"]["online"].asString() + "/" + v["players"]["max"].asString(), LOGGER_LEVEL_INFO);
		utilityPrint("Version: " + v["version"]["name"].asString(), LOGGER_LEVEL_INFO);
	}

	Client* g_Client = NULL;
}