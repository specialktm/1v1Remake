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
		Submenu("Description", &Menus::TextureDescription);
		IconButton(ICON_FA_TRASH, "Clear Cached Images/Gifs", [] { g_ImageLoader.ClearCache(); });
		Break("Menu Related");
		Float("Scrolling Animation Speed", "", &g_Renderer->Menu.m_LerpSpeed,0.001f,1.0f,0.01f);
		Bool("Move Text With Scroller", "", &g_Renderer->Menu.m_LerpText);
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