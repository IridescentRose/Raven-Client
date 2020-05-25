#include "InternalClient.h"
#include "../Utils.h"

namespace Minecraft::Client::Internal {
	InternalClient::InternalClient()
	{
	}
	InternalClient::~InternalClient()
	{
	}
	void InternalClient::start()
	{
		utilityPrint("Starting Client...", LOGGER_LEVEL_INFO);

	}
	void InternalClient::stop()
	{
	}
	void InternalClient::update()
	{
	}
	void InternalClient::draw()
	{
	}
	InternalClient* g_InternalClient;
	ClientInfo g_ClientInfo;
}