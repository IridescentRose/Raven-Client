#include "Client.h"
#include "../Config.h"
#include <iostream>

namespace Minecraft {

	Client::Client()
	{
	}

	Client::~Client()
	{
	}

	void Client::testPing(){
		Network::PacketOut* packet = new Network::PacketOut(64);
		packet->ID = 0;
		packet->buffer->WriteVarInt32(404); //1.13.2 protocol
		packet->buffer->WriteVarUTF8String(g_Config.ip);
		packet->buffer->WriteBEUInt16(g_Config.port);
		packet->buffer->WriteBEUInt8(1); //Status ping

		//Send handshake
		Network::g_NetworkDriver.AddPacket(packet);
		Network::g_NetworkDriver.SendPackets(false);

		//Assuming this works, we need to send our status request and also send a ping packet.
		Network::PacketOut* packet2 = new Network::PacketOut(1);
		packet2->ID = 0;
		//This is blank
		Network::g_NetworkDriver.AddPacket(packet2);
		Network::g_NetworkDriver.SendPackets(false);


		//Wait a good 10 ticks
		Platform::delayForMS(500);
		int timeout = 0;
		Network::PacketIn* pIn = csock->Recv(false);
		while(pIn == NULL){
			pIn = csock->Recv(false);
			Platform::delayForMS(50);
			timeout++;

			if(timeout == 100){
				throw std::runtime_error("COULDN'T CONNECT!");
			}
		}

		//So we must have received a valid client status to get here.
		std::string serverData = "";
		pIn->buffer->ReadVarUTF8String(serverData);
		std::cout << serverData << std::endl;

		delete pIn;
		pIn = NULL;

		//Ping
		Network::PacketOut* packet3 = new Network::PacketOut(9);
		packet3->ID = 0x01;
		packet3->buffer->WriteBEUInt64(0xCAFEBABE);
		Network::g_NetworkDriver.AddPacket(packet3);
		Network::g_NetworkDriver.SendPackets(false);

		pIn = csock->Recv(false);
		while (pIn == NULL) {
			pIn = csock->Recv(false);
			Platform::delayForMS(50);
			timeout++;

			if (timeout == 100) {
				throw std::runtime_error("COULDN'T GET PING!");
			}
		}

		uint64_t pingData = 0;
		pIn->buffer->ReadBEUInt64(pingData);

		std::cout << "PONG: " << std::hex << pingData << std::endl;

		csock->Close();
		//We're good

	}

	void Client::init()
	{
		Network::g_NetworkDriver.Init();

		//Connect
		csock = new Network::ClientSocket();
		csock->Connect(g_Config.port, g_Config.ip.c_str());

		Network::g_NetworkDriver.setSocket(csock);

		//At this point we're being invoked - not neutral - hence we should connect.
		//We should test ping for inquiry

		testPing();
	}

	void Client::cleanup()
	{
		Network::g_NetworkDriver.Cleanup();
	}

	void Client::update()
	{
	}

	void Client::draw()
	{
	}
}
