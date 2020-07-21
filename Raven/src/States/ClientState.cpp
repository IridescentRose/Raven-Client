#include "ClientState.h"

ClientState::ClientState()
{
}

ClientState::~ClientState()
{
}

void ClientState::init()
{
	client = new Minecraft::Client();
	client->init();
}

void ClientState::cleanup()
{
}

void ClientState::enter()
{
}

void ClientState::pause()
{
}

void ClientState::resume()
{
}

void ClientState::update(GameStateManager* st)
{
	client->update();
}

void ClientState::draw(GameStateManager* st)
{
	client->draw();
}
