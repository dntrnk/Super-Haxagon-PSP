#include "Core/Game.hpp"
#include "Driver/Platform.hpp"

#include <pspkernel.h>

PSP_MODULE_INFO("Super Haxagon", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#ifdef _WIN64
int WinMain() {
#else
int main(int, char**) {
#endif
	SuperHaxagon::Platform platform;
	platform.message(SuperHaxagon::Dbg::INFO, "main", "starting main");

	if (platform.loop()) {
		SuperHaxagon::Game game(platform);
		game.run();
	}

	platform.message(SuperHaxagon::Dbg::INFO, "main", "stopping main");
	platform.shutdown();

	return 0;
}
