#include "../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{
	void Submenus::Menus::MenuSettings()
	{
		Title("Settings");
		NewSub("Theme", Menus::TextureThemes);
		//Float("Theme", "LOL", &g_Renderer->Menu.Item.m_BreakUnderline.x,0.f,200.f,1.f);
		Break("Custom Textures");
		NewSub("Header", Menus::TextureHeader);
		NewSub("Subtitle", Menus::TextureSubtitle);
		NewSub("Background", Menus::TextureBackground);
		NewSub("Scroller", Menus::TextureScroller);
		NewSub("Footer", Menus::TextureFooter);
		Break("Reset");
		Button("Save Theme", [&]
		{
			queue::add([&] {
				g_ThemeLoader.SaveTheme();
			});
		});	
		Button("Save Custom Textures", [&]
		{
			queue::add([&] {
				g_ThemeLoader.SaveThemeFile();
			});
		});
		Button("Reset Textures", [&]
		{
			g_ThemeLoader.Reset(); 
		});
	}
}