#include "../renderer.hpp"
#include "submenu/Submenu.h"

namespace cheat
{
	inline char buffer[400];
	void Submenus::Home()
	{
		Title("Home");
		NewSub("Players", Menus::Players);
		NewSub("ESP", Menus::MenuESP);
		NewSub("Settings", Menus::MenuSettings);
		g_Renderer->Menu.Item.AddTextInput("Theme Name", "Theme Save Name", "Enter Name", buffer, sizeof(buffer));
		Button("Save Folder Theme", [] {g_ThemeLoader.SaveThemeToFolder(buffer); });
	}
}