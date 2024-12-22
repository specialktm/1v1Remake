#include "main.h"
#include "renderer/renderer.hpp"
#include "hooking/hooking.h"
#include "../kiero/kiero.h"
#include "renderer/ThemeLoader/ThemeLoader.h"
#include "../util/Il2cpp_Resolver/Il2cpp_Resolver.hpp"

#ifdef Developer
/*	
	TODO:
	    [T] Fix FA (FontAwesome),
	    [T] Add More Features (e.g Aimbot, Godmode, etc)

	Changelog: Commit 14
		[+] Image Cache

	Changelog: Commit 13
		[+] Full Custom Theme Support
	
	Changelog: Commit 12
		[+] Theme Loading (with json),
		[+] Theme Saving (with json)
	
	Features:
		[1] Skeleton ESP,
		[2] Box ESP,
		[3] Full Menu Customization,
		[4] Theme Loading/Saving through Folders and Json,
		[5] Custom Themes
		[6] No Recoil (can't toggle maybe future)
*/
#endif
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
		g_Hooking.reset(nullptr);
		g_Renderer.reset(nullptr);
		g_logger.reset(nullptr);

		CloseHandle(g_MainThread);
		FreeLibraryAndExitThread(g_Module,0);
	}

}