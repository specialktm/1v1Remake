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
		void LoadHeader(const fs::path& file);
		void LoadSubtitle(const fs::path& file);
		void LoadBackground(const fs::path& file);
		void LoadScroller(const fs::path& file);
		void LoadFooter(const fs::path& file);
		void LoadDescription(const fs::path& file);
		void ResetTheme();
		void LoadTextureFromEnum(ImageType Enum, const fs::path& file);

		std::vector<fs::directory_entry> GetFiles(ImageType type);
		std::vector<fs::directory_entry> GetThemes();
		fs::path GetPath(ImageType type);
	private:
		void CreateFolders();
		fs::path GetDocumentsPath();

		fs::path DocumentsFolder{ GetDocumentsPath() / "CortezMenu"};
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
			const char* ext = filePath.extension().string().c_str();
			if (std::ranges::find(ValidExtensions.begin(), ValidExtensions.end(), ext) != ValidExtensions.end())
			{
				return true;
			}
			for (const auto& extension : ValidExtensions.end())
			{
				g_logger->send(levels::error, "Invalid extension: {}", extension);
			}
			return false;
		}
		
	};
	inline ThemeLoader g_ThemeLoader;
}