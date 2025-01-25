#include "../renderer.hpp"
#include "submenu/Submenu.h"

namespace cheat
{
	inline char ThemeNameBuffer[400];

	void Submenus::Home()
	{
		Title("Home");
		Submenu("Players", &Menus::Players);
		Submenu("No Skill", &Menus::MenuESP, ICON_FA_BINOCULARS);
		Submenu("Settings", &Menus::MenuSettings, ICON_FA_COG);
		Break();
		Input("Theme Name", "Theme Save Name", "Enter Name", ThemeNameBuffer, sizeof(ThemeNameBuffer));
		Button("Save Theme", []
		{
			if (ThemeNameBuffer != "")
			{
				std::string editBuffer{ ThemeNameBuffer };
				editBuffer.replace(editBuffer.find("5"), 1, "");
				g_ThemeLoader.SaveThemeToFolder(editBuffer.c_str());
				editBuffer.clear();
			}
		});
	}
}