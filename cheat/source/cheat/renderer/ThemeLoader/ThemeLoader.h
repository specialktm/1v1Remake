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
		fs::path Description;
		
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

	enum class ImageType : int
	{
		Header = 0,
		Subtitle,
		Background,
		Scroller,
		Footer,
		Description

	};

	class ThemeLoader 
	{
	public:
		void CreateFolders();

		void LoadTheme(const fs::path& folder);
		void LoadThemeFromFile(const fs::path& file);
		void SaveTheme(bool isFile = false);
		bool SaveThemeToFolder(const std::string_view& ThemeName);
		void LoadHeader(const fs::path& file);
		void LoadSubtitle(const fs::path& file);
		void LoadBackground(const fs::path& file);
		void LoadScroller(const fs::path& file);
		void LoadFooter(const fs::path& file);
		void LoadDescription(const fs::path& file);
		void ResetTheme();

		std::vector<fs::directory_entry> GetFiles(ImageType type);
		std::vector<fs::directory_entry> GetThemes();
		fs::path GetPath(ImageType type);

	private:
		fs::path DocumentsFolder{ GetDocumentsPath() / "CortezMenu"};
		fs::path HeaderPath_{ DocumentsFolder / "Textures" / "Header" };
		fs::path SubtitlePath_{ DocumentsFolder / "Textures" / "Subtitle" };
		fs::path BackgroundPath_{ DocumentsFolder / "Textures" / "Background" };
		fs::path ScrollerPath_{ DocumentsFolder / "Textures" / "Scroller" };
		fs::path FooterPath_{ DocumentsFolder / "Textures" / "Footer" };
		fs::path DescriptionPath_{ DocumentsFolder / "Textures" / "Description" };
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