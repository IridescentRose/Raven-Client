#include "Logo.h"

#include <GFX/RenderCore.h>
Logo::Logo()
{
}
Logo::~Logo()
{
}
void Logo::init()
{
	timer = Utilities::Timer();
	logoTexture = GFX::g_TextureManager->loadTex("./assets/minecraft/textures/gui/title/mojang.png", GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, false);
	logoSprite = new GFX::Render2D::Sprite(logoTexture, { 272, 272 });
	logoSprite->setPosition(240, 136);
	timer.deltaTime();

	GFX::g_RenderCore->setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Logo::cleanup()
{
	GFX::g_TextureManager->deleteTex(logoTexture);
	delete logoSprite;
}

void Logo::enter()
{
}

void Logo::pause()
{
}

void Logo::resume()
{
}

void Logo::update(Core::GameStateManager* st)
{
	timer.deltaTime();

	if(timer.elapsed() >= 5.0f){
		//Switch state to menu.
	}
}

void Logo::draw(Core::GameStateManager* st)
{
	if(timer.elapsed() / 10.0f <= 1.0f){
		logoSprite->setColor(1.0f, 1.0f, 1.0f, (float)timer.elapsed() / 10.0f);
	}else if (timer.elapsed() / 10.0f > 1 && timer.elapsed() < 4){
		logoSprite->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	}else if (timer.elapsed() / 10.0f  <= 5.0f){
		logoSprite->setColor(1.0f, 1.0f, 1.0f, 5.0f - (float)timer.elapsed() / 10.0f);
	}
	logoSprite->draw();
}
