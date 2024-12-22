#pragma once
#include "../../../pch/pch.h"
#include "../../../common/common.h"
#include "../../hooking/hooking.h"
#include "../renderer.hpp"
#include "../ImageLoader/ImageLoader.h"

#include <json.hpp>

namespace cheat 
{

	struct ThemeFormat {
		fs::path Header;
		fs::path Subtitle;
		fs::path Background;
		fs::path Scroller;
		fs::path Footer;
	};

	enum class ImageType
	{
		Header = 1,
		Subtitle = 2,
		Background = 3,
		Scroller = 4,
		Footer = 5,

	};

	class ThemeLoader 
	{
	public:
		void CreateFolders();

		void LoadTheme(const fs::path& folder);
		void LoadThemeFromFile(const fs::path& file);
		void SaveTheme();
		void SaveThemeFile();
		void LoadHeader(const fs::path& file);
		void LoadSubtitle(const fs::path& file);
		void LoadBackground(const fs::path& file);
		void LoadScroller(const fs::path& file);
		void LoadFooter(const fs::path& file);
		void Reset();

		std::vector<fs::directory_entry> GetFiles(ImageType type);
		std::vector<fs::directory_entry> GetThemes();

	private:
	
		fs::path HeaderPath = "C:\\ImaGuy\\Header\\";
		fs::path SubtitlePath = "C:\\ImaGuy\\Subtitle\\";
		fs::path BackgroundPath = "C:\\ImaGuy\\Background\\";
		fs::path ScrollerPath = "C:\\ImaGuy\\Scroller\\";
		fs::path FooterPath = "C:\\ImaGuy\\Footer\\";
		fs::path ThemePath = "C:\\ImaGuy\\Themes\\";
		fs::path SavedThemePath = "C:\\ImaGuy\\theme.json";
		fs::path LoadedTheme = "";
		ThemeFormat ThemeData{};

		bool IsExtensionValid(const fs::path& filePath) {
			std::string ext = filePath.extension().string();
			for (const auto& validExt : ValidExtensions) {
				if (ext == validExt) {
					return true;
				}
			}
			return false;
		}
		
	};
	inline ThemeLoader g_ThemeLoader;
}