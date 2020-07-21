#pragma once
#include <Core/GameState.h>
#include <GFX/2D/Sprite.h>
#include <Utilities/Timer.h>

using namespace Stardust;
using namespace Stardust::Core;
class Logo : public GameState{
public:
	Logo();
	~Logo();

	void init();
	void cleanup();

	void enter();
	void pause();
	void resume();

	void update(GameStateManager* st);
	void draw(GameStateManager* st);

private:
	unsigned int logoTexture;
	GFX::Render2D::Sprite* logoSprite;
	Utilities::Timer timer;
};