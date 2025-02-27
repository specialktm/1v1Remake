#include "../../../renderer/Menu/Submenu/Submenu.h"
#include "../../../hooking/hooking.h"
#include "../../../discord/discord.h"
#include <shellapi.h>

namespace cheat
{

	void Submenus::Menus::MenuSettings() 
	{
		Title("Settings");
		
		IconButton(ICON_FA_FOLDER_OPEN, "Open Folder", [] 
		{
			HINSTANCE hInst = ShellExecuteW(NULL, L"explore", 
				g_ThemeLoader.GetPath(static_cast<ImageType>(404)).wstring().c_str(), NULL, NULL, SW_SHOW);

			if ((int)hInst <= ERROR_PATH_NOT_FOUND || hInst == nullptr) {
				g_logger->send(levels::error, "Failed to open folder.");
			}

		});

		Break("Watermark");
		Bool("Render Watermark", "", &g_Renderer->m_ShouldRendererWatermark);
		Break("Discord");
		Bool("Enable Discord Rich Presence", "", &g_DiscordManager.m_RichPresenceEnabled);
		Break("Saved Themes");
		Submenu("Theme", &Menus::TextureThemes);
		Break("Custom Textures");
		Submenu("Header", &Menus::TextureHeader);
		Submenu("Subtitle", &Menus::TextureSubtitle);
		Submenu("Background", &Menus::TextureBackground);
		Submenu("Scroller", &Menus::TextureScroller);
		Submenu("Footer", &Menus::TextureFooter);

		Break("Description");
		Float("Description Text Padding","Padding for the description text.",&m_DescriptionTextPadding, 1.f, 10.f, 0.1f);
		Float("Description Padding","Padding for the description.",&m_DescriptionPadding, 1.f, 20.f, 1.f);
		Break("Menu Related");
		Number("Max Options Visible", "", &g_Renderer->Menu.Item.m_NumToShow, 12, 20);
		Break("Theme Related");
		IconButton(ICON_FA_FOLDER_OPEN, "Save Theme", [&]
		{
			g_ThemeLoader.SaveTheme();
		});
		IconButton(ICON_FA_FOLDER_OPEN, "Save Custom Textures", [&]
		{
			g_ThemeLoader.SaveThemeFile();
		});
		Break("Reset");
		IconButton(ICON_FA_SYNC, "Reset Textures", [&]
		{
			g_ThemeLoader.Reset();
		});
	}
	
}