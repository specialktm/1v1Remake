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
		
		bool operator==(const ThemeFormat& Data) const {
			return Header == Data.Header &&
				Subtitle == Data.Subtitle &&
				Background == Data.Background &&
				Scroller == Data.Scroller &&
				Footer == Data.Footer;
		}

		bool operator!=(const ThemeFormat& Data) const {
			return Header != Data.Header ||
				Subtitle != Data.Subtitle ||
				Background != Data.Background ||
				Scroller != Data.Scroller ||
				Footer != Data.Footer;
		}
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
		bool SaveThemeToFolder(const std::string_view& ThemeName);
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
		fs::path DocumentsFolder{ GetDocumentsPath() / "CortezMenu"};
		fs::path HeaderPath_{ DocumentsFolder / "Textures" / "Header" };
		fs::path SubtitlePath_{ DocumentsFolder / "Textures" / "Subtitle" };
		fs::path BackgroundPath_{ DocumentsFolder / "Textures" / "Background" };
		fs::path ScrollerPath_{ DocumentsFolder / "Textures" / "Scroller" };
		fs::path FooterPath_{ DocumentsFolder / "Textures" / "Footer" };
		fs::path ThemePath_{ DocumentsFolder / "Themes" };
		fs::path SavedThemePath_{ DocumentsFolder / "theme.json" };
		fs::path LoadedTheme_ = "";
		ThemeFormat ThemeData_{};

		fs::path GetDocumentsPath();
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