#include "MenuState.h"
#include <GFX/RenderCore.h>
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
	panorama = new GFX::Render3D::Skybox(tex, 1.0f);
}

void MenuState::cleanup()
{
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
	//cam->rot.x += dt * (360.0f / 180.0f);
	//cam->rot.y = sinf((cam->rot.x + 90) / 180 * 3.14159) * 30.0f + 5.0f;
	cam->rot.x = 0;
	cam->rot.y += -30 * dt;
	cam->update();
}

void MenuState::draw(GameStateManager* st)
{
	panorama->draw();
}
