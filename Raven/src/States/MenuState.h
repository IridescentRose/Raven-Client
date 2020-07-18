#pragma once
#include <Core/GameState.h>
#include <GFX/2D/Sprite.h>
#include <GFX/3D/Camera.h>
#include <GFX/3D/Skybox.h>
#include <Utilities/Timer.h>
#include <GFX/UI/TextRenderer.h>

using namespace Stardust;
using namespace Stardust::Core;

class MenuState : public GameState{
public:
	MenuState();
	~MenuState();

	void init();
	void cleanup();

	void enter();
	void pause();
	void resume();

	void update(GameStateManager* st);
	void draw(GameStateManager* st);
	
private:
	GFX::Render3D::Camera* cam;
	GFX::Render3D::Skybox* panorama;

	unsigned int logoTexture;
	GFX::Render2D::Sprite* logoSprite;

	GFX::UI::TextRenderer* textRender;
};