#include "main.h"
#include "renderer/renderer.hpp"
#include "hooking/hooking.h"
#include "../kiero/kiero.h"
#include "renderer/ThemeLoader/ThemeLoader.h"

namespace cheat
{
	unsigned long main_entry(void*)
	{
		g_logger = std::make_unique<logger>();
		g_logger->send(levels::info, "Build Date: {}", __DATE__);

		Fiber::ensure_thread_is_a_fiber();
		queue::initialize();

		g_Hooking = std::make_unique<hooking>();
		g_Renderer = std::make_unique<renderer>();
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
		g_Renderer.reset();
		g_Hooking.reset();
		g_logger.reset();

		CloseHandle(g_MainThread);
		FreeLibraryAndExitThread(g_Module,0);
	}

}