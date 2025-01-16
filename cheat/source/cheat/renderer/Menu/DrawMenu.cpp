#include "../renderer.hpp"
#include "submenu/Submenu.h"

namespace cheat
{
	inline char buffer[400];
	void Submenus::Home()
	{
		Title("Home");
		Submenu("Players", &Menus::Players);
		Submenu("No Skill", &Menus::MenuESP, ICON_FA_BINOCULARS);
		Submenu("Settings", &Menus::MenuSettings, ICON_FA_COG);
			
		Input("Theme Name", "Theme Save Name", "Enter Name", buffer, sizeof(buffer));
		Button("Save Folder Theme", [] {g_ThemeLoader.SaveThemeToFolder(buffer); });
	}
}