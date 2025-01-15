#include "../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{
	void Submenus::Menus::MenuSettings()
	{
		Title("Settings");
		Submenu("Theme", Menus::TextureThemes);
		Break("Custom Textures");
		Submenu("Header", Menus::TextureHeader);
		Submenu("Subtitle", Menus::TextureSubtitle);
		Submenu("Background", Menus::TextureBackground);
		Submenu("Scroller", Menus::TextureScroller);
		Submenu("Footer", Menus::TextureFooter);

		Break("Description");
		Float("Description Text Padding","Padding for the description text.",&m_DescriptionTextPadding,1.f, 10.f, 0.1f);
		Float("Description Padding","Padding for the description.",&m_DescriptionPadding,1.f, 20.f, 1.f);

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