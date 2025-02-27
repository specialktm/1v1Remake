#include "ThemeLoader.h"
#include "../../../common/common.h"

#include <shlobj.h>
#include <objbase.h>
#pragma comment(lib,"Shell32")
#pragma comment(lib,"Ole32")


namespace cheat
{

	fs::path ThemeLoader::GetDocumentsPath()
	{
		wchar_t* path;
		if (S_OK != SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path)) return "";
		std::filesystem::path result = path;
		CoTaskMemFree(path);

		return path;
	}


	bool CreateThemeFolders(fs::path& ThemePath)
	{
		fs::path HeaderPath{ ThemePath / "Header" };
		fs::path SubtitlePath{ ThemePath / "Subtitle" };
		fs::path BackgroundPath{ ThemePath / "Background" };
		fs::path ScrollerPath{ ThemePath / "Scroller" };
		fs::path FooterPath{ ThemePath / "Footer" };

		std::vector<fs::path> paths = { HeaderPath, SubtitlePath, BackgroundPath, ScrollerPath, FooterPath, ThemePath };
		for (const auto& folder : paths)
		{
			if (!fs::exists(folder))
			{
				if (!fs::create_directories(folder))
				{
					g_logger->send(levels::error, "Failed to create folder: {}", folder.string());
				}
			}
		}

		g_logger->send(levels::debug, "Made All Folders");
		return true;
	}


	void ThemeLoader::CreateFolders()
	{
		std::vector<fs::path> paths = { HeaderPath_, SubtitlePath_, BackgroundPath_, ScrollerPath_, FooterPath_, ThemePath_ };

		for (const auto& path : paths)
		{
			if (!fs::exists(path))
			{
				try
				{
					fs::create_directories(path);
					g_logger->send(levels::success, "Created directory: {}", path.string());
				}
				catch (const std::exception& e)
				{
					g_logger->send(levels::error, "Exception while creating directory {}: {}", path.string(), e.what());
				}
			}
			else
			{
				g_logger->send(levels::debug, "Directory already exists: {}", path.string());
			}
		}

		if (fs::exists(SavedThemePath_))
		{
			LoadThemeFromFile(SavedThemePath_);
		}
	}


	void ThemeLoader::LoadHeader(const fs::path& file)
	{
		ThemeData_.Header.clear();
		g_Renderer->Menu.Header.m_Header.clear();
		g_Renderer->Menu.Header.m_HeaderFrame = 0;

		if (file.extension() == ".gif")
		{
			g_Renderer->Menu.Header.m_Header = g_ImageLoader.CreateGifTexture(D3D11::m_Device.Get(), file);
		}
		else
		{
			g_Renderer->Menu.Header.m_Header.try_emplace(0, 0, g_ImageLoader.CreateTexture(D3D11::m_Device.Get(), file));
		}
		ThemeData_.Header = file;
	}

	void ThemeLoader::LoadSubtitle(const fs::path& file)
	{
		ThemeData_.Subtitle.clear();
		g_Renderer->Menu.Item.m_SubtitleImage.clear();
		g_Renderer->Menu.Item.m_SubtitleFrame = 0;

		if (file.extension() == ".gif")
		{
			g_Renderer->Menu.Item.m_SubtitleImage = g_ImageLoader.CreateGifTexture(D3D11::m_Device.Get(), file);
		}
		else
		{
			g_Renderer->Menu.Item.m_SubtitleImage.try_emplace(0, 0, g_ImageLoader.CreateTexture(D3D11::m_Device.Get(), file));
		}
		ThemeData_.Subtitle= file;
	}

	void ThemeLoader::LoadBackground(const fs::path& file)
	{
		ThemeData_.Background.clear();
		g_Renderer->Menu.Item.m_BackgroundImage.clear();
		g_Renderer->Menu.Item.m_BackgroundFrame = 0;
		if (file.extension() == ".gif")
		{
			g_Renderer->Menu.Item.m_BackgroundImage = g_ImageLoader.CreateGifTexture(D3D11::m_Device.Get(), file);
		}
		else
		{
			g_Renderer->Menu.Item.m_BackgroundImage.try_emplace(0, 0, g_ImageLoader.CreateTexture(D3D11::m_Device.Get(), file));

		}
		ThemeData_.Background = file;
	}

	void ThemeLoader::LoadScroller(const fs::path& file)
	{
		ThemeData_.Scroller.clear();
		g_Renderer->Menu.Item.m_Image.clear();
		g_Renderer->Menu.Item.m_ScrollerFrame = 0;
		if (file.extension() == ".gif")
		{
			g_Renderer->Menu.Item.m_Image = g_ImageLoader.CreateGifTexture(D3D11::m_Device.Get(), file);
		}
		else
		{
			g_Renderer->Menu.Item.m_Image.try_emplace(0, 0, g_ImageLoader.CreateTexture(D3D11::m_Device.Get(), file));

		}
		ThemeData_.Scroller = file;
	}
	void ThemeLoader::LoadFooter(const fs::path& file)
	{
		ThemeData_.Footer.clear();

		g_Renderer->Menu.Item.m_FooterImage.clear();
		g_Renderer->Menu.Item.m_FooterFrame = 0;

		if (file.extension() == ".gif")
		{
			g_Renderer->Menu.Item.m_FooterImage = g_ImageLoader.CreateGifTexture(D3D11::m_Device.Get(), file);
		}
		else
		{
			g_Renderer->Menu.Item.m_FooterImage.try_emplace(0, 0, g_ImageLoader.CreateTexture(D3D11::m_Device.Get(), file));
		}
		ThemeData_.Footer = file;
	}

	void ThemeLoader::LoadTheme(const fs::path& folder) 
	{
		Reset();
		std::unordered_map<std::string, void (ThemeLoader::*)(const fs::path&)> loadFunctions = {
				{"Header", &ThemeLoader::LoadHeader},
				{"Subtitle", &ThemeLoader::LoadSubtitle},
				{"Background", &ThemeLoader::LoadBackground},
				{"Scroller", &ThemeLoader::LoadScroller},
				{"Footer", &ThemeLoader::LoadFooter}
		};

		for (const auto& [subfolderName, loadFunction] : loadFunctions) 
		{
			fs::path subfolderPath = folder / subfolderName;

			if (fs::exists(subfolderPath) && fs::is_directory(subfolderPath)) 
			{
				for (const auto& entry : fs::directory_iterator(subfolderPath)) {
					
					if (entry.is_regular_file() && IsExtensionValid(entry.path())) {
						(this->*loadFunction)(entry.path());
						break;
					}
				}
			}
			else 
			{
				g_logger->send(levels::error, "Subfolder not found or not a directory: {}", subfolderPath.string());
			}

		}
		LoadedTheme_ = folder;
		g_logger->send(levels::success, "Theme Loaded: {}", folder.string().c_str());
	}

	std::vector<fs::directory_entry> ThemeLoader::GetFiles(ImageType type)
	{
		std::vector<fs::directory_entry> tmp_vec;
		fs::directory_iterator target_path;

		try
		{
			switch (type)
			{
			case ImageType::Header:
				target_path = fs::directory_iterator(HeaderPath_);
				break;
			case ImageType::Subtitle:
				target_path = fs::directory_iterator(SubtitlePath_);
				break;
			case ImageType::Background:
				target_path = fs::directory_iterator(BackgroundPath_);
				break;
			case ImageType::Scroller:
				target_path = fs::directory_iterator(ScrollerPath_);
				break;
			case ImageType::Footer:
				target_path = fs::directory_iterator(FooterPath_);
				break;
			default:
				return tmp_vec;
			}
			for (const auto& entry : target_path)
			{
				tmp_vec.push_back(entry);
			}

		}

		catch (const fs::filesystem_error& e)
		{
			MessageBoxA(D3D11::m_window, "Filesystem error", e.what(), 1);
		}
		catch (const std::exception& e)
		{
			MessageBoxA(D3D11::m_window, "Filesystem error", e.what(), 1);
			g_logger->send(levels::error, "General exception: {}", e.what());
		}

		return tmp_vec;
	}

	fs::path ThemeLoader::GetPath(ImageType type)
	{
		fs::path target_path;

		try
		{
			switch (type)
			{
			case ImageType::Header:
				target_path = HeaderPath_;
				break;
			case ImageType::Subtitle:
				target_path = SubtitlePath_;
				break;
			case ImageType::Background:
				target_path = BackgroundPath_;
				break;
			case ImageType::Scroller:
				target_path = ScrollerPath_;
				break;
			case ImageType::Footer:
				target_path = FooterPath_;
				break;
			default:
				return DocumentsFolder;
			}
		}

		catch (const fs::filesystem_error& e)
		{
			MessageBoxA(D3D11::m_window, "Filesystem error", e.what(), 1);
		}
		catch (const std::exception& e)
		{
			MessageBoxA(D3D11::m_window, "Filesystem error", e.what(), 1);
			g_logger->send(levels::error, "General exception: {}", e.what());
		}

		return target_path;
	}

	std::vector<fs::directory_entry> ThemeLoader::GetThemes()
	{
		std::vector<fs::directory_entry> tmp_vec;
		fs::directory_iterator target_path = fs::directory_iterator(ThemePath_);

		try
		{
			for (const auto& entry : target_path)
			{
				tmp_vec.push_back(entry);
			}
		}
		catch (const fs::filesystem_error& e)
		{
			MessageBoxA(D3D11::m_window, "Filesystem error", e.what(), 1);
		}
		catch (const std::exception& e)
		{
			MessageBoxA(D3D11::m_window, "Filesystem error", e.what(), 1);
			g_logger->send(levels::error, "General exception: {}", e.what());
		}
		std::sort(tmp_vec.begin(), tmp_vec.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
			return a.path().filename() < b.path().filename();
		});
		return tmp_vec;
	}

	void ThemeLoader::SaveTheme()
	{
		nlohmann::json JsonData;
		JsonData["SavedTheme"] = LoadedTheme_.string();
		std::ofstream file{ SavedThemePath_.string() };
		if (file.is_open()) {
			file << JsonData.dump(4);
			file.close();
		}
	}

	void ThemeLoader::SaveThemeFile()
	{
		nlohmann::json JsonData;
		
		JsonData["Header"] = ThemeData_.Header.string();
		JsonData["Subtitle"] = ThemeData_.Subtitle.string();
		JsonData["Background"] = ThemeData_.Background.string();
		JsonData["Scroller"] = ThemeData_.Scroller.string();
		JsonData["Footer"] = ThemeData_.Footer.string();

		std::ofstream file{ SavedThemePath_.string() };
		if (file.is_open()) {
			file << JsonData.dump(4);
			file.close();
		}
	}

	void ThemeLoader::LoadThemeFromFile(const fs::path& file)
	{
		std::ifstream File{ file.string() };
		nlohmann::json JsonData = nlohmann::json::parse(File);
		File.close();

		if (JsonData.contains("SavedTheme") && !JsonData["SavedTheme"].empty())
		{
			if (fs::exists(JsonData["SavedTheme"]))
			{
				LoadTheme(JsonData["SavedTheme"]);
			}
			return;
		}
		else
		{
			if (JsonData.contains("Header")) LoadHeader(JsonData["Header"]);
			if (JsonData.contains("Subtitle")) LoadSubtitle(JsonData["Subtitle"]);
			if (JsonData.contains("Background")) LoadBackground(JsonData["Background"]);
			if (JsonData.contains("Scroller")) LoadScroller(JsonData["Scroller"]);
			if (JsonData.contains("Footer")) LoadFooter(JsonData["Footer"]);
		}
	}
	
	bool ThemeLoader::SaveThemeToFolder(const std::string_view& ThemeName)
	{
		 if (!std::empty(ThemeName))
		 {

			 fs::path ThemePath{ ThemePath_ / ThemeName };
			 fs::path HeaderPath{ ThemePath / "Header" / "Header" += ThemeData_.Header.extension() };
			 fs::path SubtitlePath{ ThemePath / "Subtitle" / "Subtitle" += ThemeData_.Subtitle.extension() };
			 fs::path BackgroundPath{ ThemePath / "Background" / "Background" += ThemeData_.Background.extension() };
			 fs::path ScrollerPath{ ThemePath / "Scroller" / "Scroller" += ThemeData_.Scroller.extension() };
			 fs::path FooterPath{ ThemePath / "Footer" / "Footer" += ThemeData_.Footer.extension() };
			 LoadedTheme_ = ThemePath;

			 if (!fs::exists(ThemePath))
			 {
				 if (!fs::create_directories(ThemePath))
				 {
					 g_logger->send(levels::error, "Failed to create theme directory: {}", ThemePath.string());
					 return false;
				 }

				 if (!CreateThemeFolders(ThemePath))
				 {
					 g_logger->send(levels::error, "Failed to create theme subfolders: {}", ThemePath.string());
					 return false;
				 }
			 }

			 if (ThemeData_ != ThemeFormat{})
			 {

				 auto copy_with_overwrite = [](const fs::path& src, const fs::path& dest) {
					 try {
						 if (fs::exists(src))
							 fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
						 return true;
					 }
					 catch (const std::exception& e) {
						 g_logger->send(levels::error, "Failed to copy file: {} -> {} | Error: {}", src.string(), dest.string(), e.what());
						 return false;
					 }
				 };


				 if (!copy_with_overwrite(ThemeData_.Header, HeaderPath)) return false;
				 if (!copy_with_overwrite(ThemeData_.Subtitle, SubtitlePath)) return false;
				 if (!copy_with_overwrite(ThemeData_.Background, BackgroundPath)) return false;
				 if (!copy_with_overwrite(ThemeData_.Scroller, ScrollerPath)) return false;
				 if (!copy_with_overwrite(ThemeData_.Footer, FooterPath)) return false;
			 }

			 SaveTheme();
			 g_logger->send(levels::success, "Successfully saved theme: {} at {}", ThemeName, ThemePath.string());
			 return true;
		 }
	}

	void ThemeLoader::Reset()
	{
		g_Renderer->Menu.Item.m_FooterImage.clear();
		g_Renderer->Menu.Item.m_FooterFrame = 0;
		g_Renderer->Menu.Item.m_Image.clear();
		g_Renderer->Menu.Item.m_ScrollerFrame = 0;
		g_Renderer->Menu.Item.m_BackgroundImage.clear();
		g_Renderer->Menu.Item.m_BackgroundFrame = 0;
		g_Renderer->Menu.Item.m_SubtitleImage.clear();
		g_Renderer->Menu.Item.m_SubtitleFrame = 0;
		g_Renderer->Menu.Header.m_Header.clear();
		g_Renderer->Menu.Header.m_HeaderFrame = 0;

		ThemeFormat TempFormat;
		LoadedTheme_.clear();
		ThemeData_ = TempFormat;
	}

	
}