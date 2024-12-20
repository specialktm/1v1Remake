#include "main.h"
#include "renderer/renderer.hpp"
#include "hooking/hooking.h"
#include "../kiero/kiero.h"
#include "renderer/ThemeLoader/ThemeLoader.h"
#include "../util/Il2cpp_Resolver/Il2cpp_Resolver.hpp"

/*
TODO: Fix FA
TODO: Add More Features (e.g Box ESP, Aimbot, Godmode ,etc)
*/

namespace cheat
{


	unsigned long main_entry(void*)
	{
		g_logger = std::make_unique<logger>();
		g_logger->send(levels::info, "Build Date: {}", __DATE__);
		Fiber::ensure_thread_is_a_fiber();
		queue::initialize();
		g_Renderer = std::make_unique<renderer>();
		g_Hooking = std::make_unique<hooking>();

		queue::add([&] {
			g_ThemeLoader.CreateFolders();
		}); 

		while (g_Running)
		{
			fiber_manager::tick();
			queue::tick();
			if (GetAsyncKeyState(VK_END))
			{
				g_Running = false;
			}
			std::this_thread::sleep_for(400ms);
		}
		fiber_manager::remove_all_fibers();
		g_Hooking.reset();
		g_Renderer.reset();
		g_logger.reset();

		CloseHandle(g_MainThread);
		FreeLibraryAndExitThread(g_Module,0);
	}

}