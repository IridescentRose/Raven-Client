#include "Application.h"
#include "States/Logo.h"

Application::Application()
{
	stateMan = new Core::GameStateManager();
}

Application::~Application()
{
}

void Application::init()
{
	Logo* logo = new Logo();
	logo->init();

	stateMan->addState(logo);

}

void Application::update()
{
	stateMan->update();
}

void Application::draw()
{
	stateMan->draw();
}
