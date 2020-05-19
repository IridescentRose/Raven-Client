#include <Platform/Platform.h>
#include <Graphics/RendererCore.h>
#include <Utilities/Logger.h>

PSP_MODULE_INFO("Raven Client", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;
using namespace Stardust::Utilities;
using namespace Stardust::Graphics;

int main() {
	Platform::initPlatform("Raven-Client");

#ifdef RAVEN_CLIENT_DEBUG
	app_Logger->currentLevel = LOGGER_LEVEL_TRACE;
	detail::core_Logger->currentLevel = LOGGER_LEVEL_TRACE;

	pspDebugScreenInit();
	app_Logger->log("Debug Mode Enabled!");
#endif


	while (true) {
		Platform::platformUpdate();

		//Note: Should actually count this out - but will do
		sceKernelDelayThread(50 * 1000);
	}

	Platform::exitPlatform();
	return 0;
}