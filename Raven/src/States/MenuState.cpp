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
}

void MenuState::draw(GameStateManager* st)
{
}
