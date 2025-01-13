#include "../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{
	void Submenus::Menus::MenuSettings()
	{
		Title("Settings");
		NewSub("Theme", Menus::TextureThemes);
		Break("Custom Textures");
		NewSub("Header", Menus::TextureHeader);
		NewSub("Subtitle", Menus::TextureSubtitle);
		NewSub("Background", Menus::TextureBackground);
		NewSub("Scroller", Menus::TextureScroller);
		NewSub("Footer", Menus::TextureFooter);
		Break("Reset");
		Button("Save Theme", [&]
		{
			g_ThemeLoader.SaveTheme();
		});
		Button("Save Custom Textures", [&]
		{
			g_ThemeLoader.SaveThemeFile();
		});
		Button("Reset Textures", [&]
		{
			g_ThemeLoader.Reset();
		});
	}
}