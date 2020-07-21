#pragma once
#include <Core/StateManager.h>

using namespace Stardust;
class Application {
public:
	Application();
	~Application();

	void init();
	void update();
	void draw();

	inline bool isRunning(){
		return stateMan->isRunning();
	}

private:
	Core::GameStateManager* stateMan;
};
