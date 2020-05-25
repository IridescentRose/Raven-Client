#pragma once
#include <cstdint>
#include <string>
namespace Minecraft::Client::Internal {
	class InternalClient {
	public:
		InternalClient();
		~InternalClient();

		void start();
		void stop();

		void update();
		void draw();
	};

	extern InternalClient* g_InternalClient;

	struct ClientInfo {
		int eid;
		uint8_t gamemode;
		int dimension;
		uint8_t difficulty;
		uint8_t maxPlayers;
		std::string levelType;
		bool reducedInfo;

		bool invulnerable, flying, allowed_fly, creative;

		float flySpeed, fovMod;

		uint8_t hotbarSlot;
	};
	extern ClientInfo g_ClientInfo;
}