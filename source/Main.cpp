// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include <pspkernel.h>

#include "Core/Game.hpp"
#include "Driver/Platform.hpp"

PSP_MODULE_INFO("Super Haxagon", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void* common) {
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void* argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

#ifdef _WIN64
int WinMain() {
#else
int main(int, char**) {
#endif
	setup_callbacks();

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
