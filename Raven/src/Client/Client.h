#pragma once
#include <Network/NetworkDriver.h>
using namespace Stardust;
#include <GFX/2D/Sprite.h>
#include <GFX/UI/TextRenderer.h>
#include "../Common/Translation.h"

namespace Minecraft{
	class Client{
	public:
		Client();
		~Client();

		void init();
		void cleanup();

		void login();

		void testPing();
		void update();
		void draw();

		void drawWaitStage(std::string keyCodes);
	private:
		Network::ClientSocket* csock;

		unsigned int options_bg;
		GFX::Render2D::Sprite* tileSprite;
		GFX::UI::TextRenderer* textRenderer;
	};
}