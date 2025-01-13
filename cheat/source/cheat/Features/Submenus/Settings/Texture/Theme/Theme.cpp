#include "../../../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{
	void Submenus::Menus::TextureThemes()
	{
		Title("Themes");
		for (const auto& folder : g_ThemeLoader.GetThemes())
		{
			Button(std::format("Load: {}", folder.path().stem().string()).c_str(), [folder] {
				g_ThemeLoader.LoadTheme(folder.path());
			});
		}
	}
}