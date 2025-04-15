#include "../renderer.hpp"
#include "submenu/Submenu.h"
#include "../notification/notification.h"

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
		Button("LL",[]
		{ 
			notify(Info, 3s,"AA");
		});		
		Button("LL", []
		{
			notify(Info, 3s, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
		}, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
		
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