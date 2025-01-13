#include "../../../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{

	void Submenus::Menus::TextureHeader()
	{
		Title("Header");
		Button("Reset Texture", [&]
		{
			g_Renderer->Menu.Header.m_Header.clear();
			g_Renderer->Menu.Header.m_HeaderFrame = 0;
		});


		Break("Rounding");
		Bool("Rounded", "Round the header", &g_Renderer->Menu.Header.m_Rounded);
		if (g_Renderer->Menu.Header.m_Rounded)
		{
			Float("Rounding", "Amount that the Header is rounded", &g_Renderer->Menu.Header.m_Rounding, 0.f, 100.f, 1.f);
		}

		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Header))
		{
			const auto path = ent.path();
			if (ent.is_regular_file() && path.has_filename())
			{
				Button(std::format("Load: {}", path.filename().string()).c_str(), [path] 
				{
					g_ThemeLoader.LoadHeader(path);
				});
			}
		}

	}

	void Submenus::Menus::TextureSubtitle()
	{
		Title("Subtitle");
		Button("Reset Texture", [&]
		{
			g_Renderer->Menu.Item.m_SubtitleImage.clear();
			g_Renderer->Menu.Item.m_SubtitleFrame = 0;
		});
		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Subtitle))
		{
			const auto path = ent.path();
			if (ent.is_regular_file())
			{
				Button(std::format("Load: {}", path.filename().string()).c_str(), [path]
				{
					g_ThemeLoader.LoadSubtitle(path);
				});
			}
		}

	}

	void Submenus::Menus::TextureBackground()
	{
		Title("Background");
		Button("Reset Texture", [&]
		{
			g_Renderer->Menu.Item.m_BackgroundImage.clear();
			g_Renderer->Menu.Item.m_BackgroundFrame = 0;
		});

		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Background))
		{
			const auto path = ent.path();
			if (ent.is_regular_file())
			{
				Button(std::format("Load: {}", path.filename().string()).c_str(), [path]
				{
					g_ThemeLoader.LoadBackground(path);
				});
			}
		}

	}

	void Submenus::Menus::TextureScroller()
	{
		Title("Scroller");
		Button("Reset Texture", [&]
		{
			g_Renderer->Menu.Item.m_Image.clear();
			g_Renderer->Menu.Item.m_ScrollerFrame = 0;
		});
		Break("Rounding");
		Bool("Rounded", "Round the scroller", &g_Renderer->Menu.Item.m_Rounded);
		if (g_Renderer->Menu.Item.m_Rounded)
		{
			Float("Rounding", "Amount that the scroller is rounded", &g_Renderer->Menu.Item.m_Rounding, 0.f, 100.f, 1.f);
		}

		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Scroller))
		{
			const auto path = ent.path();
			if (ent.is_regular_file())
			{
				Button(std::format("Load: {}", path.filename().string()).c_str(), [path]
				{
					g_ThemeLoader.LoadScroller(path);
				});
			}
		}

	}

	void Submenus::Menus::TextureFooter()
	{
		Title("Footer");
		Button("Reset Texture", [&] {
			g_Renderer->Menu.Item.m_FooterImage.clear();
			g_Renderer->Menu.Item.m_FooterFrame = 0;
		});
		Break("Rounding");
		Bool("Rounded", "Round the footer", &g_Renderer->Menu.Item.m_FooterRounded);
		if (g_Renderer->Menu.Item.m_FooterRounded)
		{
			Float("Rounding", "Amount that the footer is rounded", &g_Renderer->Menu.Item.m_FooterRounding, 0.f, 100.f, 1.f);
		}

		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Footer))
		{
			const auto path = ent.path();
			if (ent.is_regular_file() && path.has_filename())
			{
				Button(std::format("Load: {}", path.filename().string()).c_str(), [path]
				{
					g_ThemeLoader.LoadFooter(path);
				});

			}
		}

	}
}