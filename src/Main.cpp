#include <Platform/Platform.h>
#include <Graphics/RendererCore.h>
#include <Utilities/Logger.h>
#include "Client.h"
#include <Utilities/Logger.h>
#include <Utilities/Timer.h>
#include <Graphics/2D/SpriteBase.h>

PSP_MODULE_INFO("Raven Client", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;
using namespace Stardust::Utilities;
using namespace Stardust::Graphics;
using namespace Minecraft::Client;

void doSplash() {
	//2D Mode initialization
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumOrtho(0, 480, 272, 0, -30, 30);
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();

	Stardust::Graphics::g_RenderCore.SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//splash screen!
	Stardust::Graphics::Texture* tex = Stardust::Graphics::TextureUtil::LoadPng("./assets/minecraft/textures/gui/title/mojang.png");

	Stardust::Graphics::Render2D::Sprite* sprite = new Stardust::Graphics::Render2D::Sprite(tex);
	sprite->SetPosition(240, 136);
	sprite->Scale(1.0f, 1.0f);

	float time = 3.0f;
	Stardust::Utilities::Timer* timer = new Stardust::Utilities::Timer();
	timer->reset();

	Stardust::Graphics::g_RenderCore.SetClearColor(255, 255, 255, 255);

	while (time > 0.0f) {
		Stardust::Graphics::g_RenderCore.BeginCommands();
		Stardust::Graphics::g_RenderCore.Clear();

		time -= timer->deltaTime();
		sprite->Draw();

		Stardust::Graphics::g_RenderCore.EndCommands();
	}

	delete sprite;
	delete timer;
	delete tex;
}
#include "State/StateManager.h"
#include "Menu/MenuState.hpp"

int main() {
	Platform::initPlatform("Raven-Client");
	doSplash();

	StateManager stateManager;
	stateManager.running = true;

	Minecraft::Menus::MenuState* state = new Minecraft::Menus::MenuState();
	state->Init();
	stateManager.ChangeState(state);

	while (stateManager.running)
	{
		stateManager.Update();
		stateManager.Draw();
	}

	sceKernelExitGame();

	return 0;
}