#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include "Application.h"

using namespace Stardust;
int main(){
	Platform::initPlatform();

	Application* app = new Application();
	app->init();

	while(app->isRunning()){

		Platform::platformUpdate();
		app->update();

		GFX::g_RenderCore->beginFrame();

		app->draw();

		GFX::g_RenderCore->endFrame();

	}

	Platform::exitPlatform();
}