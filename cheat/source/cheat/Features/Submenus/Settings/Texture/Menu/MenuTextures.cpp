#include "../../../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{

	void Submenus::Menus::TextureHeader()
	{
		Title("Header");
		IconButton(ICON_FA_SYNC, "Reset Texture", [&]
		{
			g_Renderer->Menu.Header.m_Header.clear();
			g_Renderer->Menu.Header.m_HeaderFrame = 0;
			auto l = g_ImageLoader.GetTextureCache();
			for (const auto& [k, v] : l) {
				g_logger->send(levels::debug, "Cache key: {}", k);
			}

		});


		Break("Rounding");
		Bool("Rounded", "Round the header", &g_Renderer->Menu.Header.m_Rounded);
		if (g_Renderer->Menu.Header.m_Rounded)
		{
			Float("Rounding", "Amount that the Header is rounded", &g_Renderer->Menu.Header.m_Rounding, 0.0f, 100.f, 1.f);
		}

		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Header))
		{
			const auto path = ent.path();
			if (ent.is_regular_file() && path.has_filename())
			{
				IconButton(ICON_FA_IMAGE, std::format("Load: {} {}", path.filename().string(), g_ImageLoader.IsCached(path.filename().string().c_str()) ? "(Cached)" : "").c_str(), [path]
				{
					g_ThemeLoader.LoadTextureFromEnum(ImageType::Header, path);
				});
			}
		}

	}

	void Submenus::Menus::TextureSubtitle()
	{
		Title("Subtitle");
		IconButton(ICON_FA_SYNC, "Reset Texture", [&]
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
				IconButton(ICON_FA_IMAGE,std::format("Load: {} {}", path.filename().string(), g_ImageLoader.IsCached(path.filename().string().c_str()) ? "(Cached)" : "").c_str(), [path]
				{
					g_ThemeLoader.LoadTextureFromEnum(ImageType::Subtitle,path);
				});
			}
		}

	}

	void Submenus::Menus::TextureBackground()
	{
		Title("Background");
		IconButton(ICON_FA_SYNC, "Reset Texture", [&]
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
				IconButton(ICON_FA_IMAGE, std::format("Load: {} {}", path.filename().string(), g_ImageLoader.IsCached(path.filename().string().c_str()) ? "(Cached)" : "").c_str(), [path]
				{
					g_ThemeLoader.LoadTextureFromEnum(ImageType::Background,path);
				});
			}
		}

	}

	void Submenus::Menus::TextureScroller()
	{
		Title("Scroller");
		IconButton(ICON_FA_SYNC, "Reset Texture", [&]
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
				IconButton(ICON_FA_IMAGE, std::format("Load: {} {}", path.filename().string(), g_ImageLoader.IsCached(path.filename().string().c_str()) ? "(Cached)" : "").c_str(), [path]
				{
					g_ThemeLoader.LoadTextureFromEnum(ImageType::Scroller,path);
				});
			}
		}

	}

	void Submenus::Menus::TextureFooter()
	{
		Title("Footer");
		IconButton(ICON_FA_SYNC, "Reset Texture", [&] {
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
				IconButton(ICON_FA_IMAGE, std::format("Load: {} {}", path.filename().string(), g_ImageLoader.IsCached(path.filename().string().c_str()) ? "(Cached)" : "").c_str(), [path]
				{
					g_ThemeLoader.LoadTextureFromEnum(ImageType::Footer,path);
				});

			}
		}
	}
	void Submenus::Menus::TextureDescription()
	{
		Title("Description");
		IconButton(ICON_FA_SYNC, "Reset Texture", [&]
		{
			g_Renderer->Menu.Item.m_DescriptionImage.clear();
			g_Renderer->Menu.Item.m_DescriptionFrame = 0;
		});
		Break("Rounding");
		Bool("Rounded Enabled", "Round the Description", &g_Renderer->Menu.Item.m_DescriptionRounded);
		if (g_Renderer->Menu.Item.m_DescriptionRounded)
		{
			Float("Rounding", "Amount that the Description is rounded", &g_Renderer->Menu.Item.m_DescriptionRounding, 0.f, 100.f, 1.f);
		}
		Break("Outline");
		Bool("Outline Enabled", "Round the Description", &g_Renderer->Menu.Item.m_DescriptionOutlineEnaled);
		Break("Padding");
		Float("Description Text Padding", "Padding for the description text.", &m_DescriptionTextPadding, 1.f, 10.f, 0.1f);
		Float("Description Padding", "Padding for the description.", &m_DescriptionPadding, 1.f, 20.f, 1.f);
		Break("Textures");
		for (const auto& ent : g_ThemeLoader.GetFiles(ImageType::Description))
		{
			const auto path = ent.path();
			if (ent.is_regular_file() && path.has_filename())
			{
				IconButton(ICON_FA_IMAGE, std::format("Load: {} {}", path.filename().string(), g_ImageLoader.IsCached(path.filename().string().c_str()) ? "(Cached)" : "").c_str(), [path]
				{
					g_ThemeLoader.LoadTextureFromEnum(ImageType::Description, path);
				});

			}
		}
	}
}