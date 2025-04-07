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
		void LoadTheme(const fs::path& folder);
		void LoadThemeFromFile(const fs::path& file);
		void SaveTheme(bool isFile = false);
		bool SaveThemeToFolder(const std::string_view& ThemeName);
		bool LoadHeader(const fs::path& file);
		bool LoadSubtitle(const fs::path& file);
		bool LoadBackground(const fs::path& file);
		bool LoadScroller(const fs::path& file);
		bool LoadFooter(const fs::path& file);
		bool LoadDescription(const fs::path& file);
		void ResetTheme();
		void LoadTextureFromEnum(ImageType Enum, const fs::path& file);

		std::vector<fs::directory_entry> GetFiles(ImageType type);
		std::vector<fs::directory_entry> GetThemes();
		fs::path GetPath(ImageType type);
		void CreateFolders();
	private:
		fs::path GetDocumentsPath();

		fs::path DocumentsFolder{ GetDocumentsPath() / "CortezMenu" };
		fs::path HeaderPath_{ DocumentsFolder / "Textures" / "Header" };
		fs::path SubtitlePath_{ DocumentsFolder / "Textures" / "Subtitle" };
		fs::path BackgroundPath_{ DocumentsFolder / "Textures" / "Background" };
		fs::path ScrollerPath_{ DocumentsFolder / "Textures" / "Scroller" };
		fs::path FooterPath_{ DocumentsFolder / "Textures" / "Footer" };
		fs::path DescriptionPath_{ DocumentsFolder / "Textures" / "Description" };
		fs::path ThemePath_{ DocumentsFolder / "Themes" };
		fs::path SavedThemePath_{ DocumentsFolder / "theme.simp" };
		fs::path LoadedTheme_ = "";
		ThemeFormat ThemeData_{};

		bool IsExtensionValid(const fs::path& filePath) {
			const std::string ext = filePath.extension().string();
			return std::ranges::find(ValidExtensions, ext) != ValidExtensions.end();
		}

	};
	inline ThemeLoader g_ThemeLoader;
}