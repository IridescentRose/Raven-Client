#include "MenuState.h"
#include <GFX/RenderCore.h>

#if CURRENT_PLATFORM == PLATFORM_PSP
#define TEXT_SIZE 0.5f
#else
#define TEXT_SIZE 0.75f
#endif

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

void MenuState::init()
{
	GFX::g_RenderCore->setClearColor(0.f, 0.f, 0.f, 1.f);

	cam = new GFX::Render3D::Camera({ 0, 0, 0 }, { 0, 0, 0 }, 90.0f, 480.0f / 272.0f, 0.1f, 1000.0f);
	GFX::g_RenderCore->bindCamera(cam);

	std::array<std::string, 6> tex;
	for(int i = 0; i < 6; i++){
		tex[i] = "./assets/minecraft/textures/gui/title/background/panorama_" + std::to_string(i) + ".png";
	}

	Utilities::g_AppTimer.deltaTime();
	panorama = new GFX::Render3D::Skybox(tex, 0.5f);

	logoTexture = GFX::g_TextureManager->loadTex("./assets/minecraft/textures/gui/title/raven.png", GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);
	logoSprite = new GFX::Render2D::Sprite(logoTexture);
	logoSprite->setScale(0.75f, 0.75f);
	logoSprite->setPosition( (1.f / 0.75f) * 240, 96);

	textRender = new GFX::UI::TextRenderer();
	textRender->init("./assets/fonts/font.pgf");
}

void MenuState::cleanup()
{
	textRender->cleanup();
	delete cam;
	cam = NULL;
	GFX::g_RenderCore->bindCamera(NULL);
	delete logoSprite;
	delete textRender;
	delete panorama;
	GFX::g_TextureManager->deleteTex(logoTexture);
}

void MenuState::enter()
{

}

void MenuState::pause()
{
}

void MenuState::resume()
{
}

void MenuState::update(GameStateManager* st)
{
	double dt =	Utilities::g_AppTimer.deltaTime();
	cam->rot.y += dt * (360.0f / 180.0f);
	cam->rot.x = sinf((cam->rot.y + 90) / 180 * 3.14159) * 30.0f + 5.0f;
	cam->update();
}

void MenuState::draw(GameStateManager* st)
{
	GFX::g_RenderCore->set3DMode();
	panorama->draw();

	GFX::g_RenderCore->setDefault2DMode();
	logoSprite->draw();

	textRender->setStyle({ 255, 255, 255, 255, TEXT_SIZE, TEXT_RENDERER_BOTTOM, TEXT_RENDERER_LEFT, 0.0f, true });
	textRender->draw("Minecraft 1.13.2 - compatible", { 0, 270 });

	textRender->setStyle({ 255, 255, 255, 255, TEXT_SIZE, TEXT_RENDERER_BOTTOM, TEXT_RENDERER_RIGHT, 0.0f, true });
	textRender->draw("Fan Version. Not Made by Mojang!", { 480, 270 });
}
