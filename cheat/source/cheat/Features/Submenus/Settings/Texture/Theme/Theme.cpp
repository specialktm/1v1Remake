#include "../../../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{
	void Submenus::Menus::TextureThemes()
	{
		Title("Themes");
		for (const auto& folder : g_ThemeLoader.GetThemes())
		{
			if (g_Renderer->Menu.Item.AddText(std::format("Load: {}", folder.path().stem().string()).c_str()))
			{
				queue::add([folder] {
					g_ThemeLoader.LoadTheme(folder.path());
				});
			}
		}

	}
}