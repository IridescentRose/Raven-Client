#pragma once
#include <Core/GameState.h>
#include "../Client/Client.h"

using namespace Stardust;
using namespace Stardust::Core;

class ClientState : public GameState {
public:
	ClientState();
	~ClientState();

	void init();
	void cleanup();

	void enter();
	void pause();
	void resume();

	void update(GameStateManager* st);
	void draw(GameStateManager* st);

private:
	Minecraft::Client* client;
};