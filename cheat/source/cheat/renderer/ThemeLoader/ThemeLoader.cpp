#include "ThemeLoader.h"
#include "../../../common/common.h"
namespace cheat
{

	void ThemeLoader::CreateFolders()
	{
		std::vector<fs::path> paths = { HeaderPath, SubtitlePath, BackgroundPath, ScrollerPath, FooterPath, ThemePath };

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

		if (fs::exists(SavedThemePath))
		{
			LoadThemeFromFile(SavedThemePath);
		}
	}


	void ThemeLoader::LoadHeader(const fs::path& file)
	{
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
		ThemeData.Header = file;
	}

	void ThemeLoader::LoadSubtitle(const fs::path& file)
	{
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
		ThemeData.Subtitle= file;
	}

	void ThemeLoader::LoadBackground(const fs::path& file)
	{
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
		ThemeData.Background = file;
	}

	void ThemeLoader::LoadScroller(const fs::path& file)
	{
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
		ThemeData.Scroller = file;
	}
	void ThemeLoader::LoadFooter(const fs::path& file)
	{
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
		ThemeData.Footer = file;
	}

	void ThemeLoader::LoadTheme(const fs::path& folder) 
	{
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

			LoadedTheme = folder;
		}
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
				target_path = fs::directory_iterator(HeaderPath);
				break;
			case ImageType::Subtitle:
				target_path = fs::directory_iterator(SubtitlePath);
				break;
			case ImageType::Background:
				target_path = fs::directory_iterator(BackgroundPath);
				break;
			case ImageType::Scroller:
				target_path = fs::directory_iterator(ScrollerPath);
				break;
			case ImageType::Footer:
				target_path = fs::directory_iterator(FooterPath);
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


	std::vector<fs::directory_entry> ThemeLoader::GetThemes()
	{
		std::vector<fs::directory_entry> tmp_vec;
		fs::directory_iterator target_path = fs::directory_iterator(ThemePath);

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
		JsonData["SavedTheme"] = LoadedTheme.string();
		std::ofstream file{ SavedThemePath.string() };
		if (file.is_open()) {
			file << JsonData.dump(4);
			file.close();
		}
	}

	void ThemeLoader::SaveThemeFile()
	{
		nlohmann::json JsonData;
		
		JsonData["Header"] = ThemeData.Header.string();
		JsonData["Subtitle"] = ThemeData.Subtitle.string();
		JsonData["Background"] = ThemeData.Background.string();
		JsonData["Scroller"] = ThemeData.Scroller.string();
		JsonData["Footer"] = ThemeData.Footer.string();

		std::ofstream file{ SavedThemePath.string() };
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
			LoadTheme(JsonData["SavedTheme"]);
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
	}
}