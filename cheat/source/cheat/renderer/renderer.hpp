#pragma once
#include "../../pch/pch.h"
#include "Menu/Menu.hpp"
#include "../../util/logger/logger.hpp"
#include "ThemeLoader/ThemeLoader.h"
#include "Menu/Submenu/Submenu.h"

namespace cheat
{

	namespace Submenus
	{
		void Home();
	}

	class renderer 
	{
	public:
		~renderer();
		void OnPresent();
		void Style();
		void ESP();
	public:
		bool m_Open = true;
		C_ImMMenu Menu;
		// Submenus
        void* currentMenu = Submenus::Home;;
        std::vector<void*> menusArray;
        std::vector<int> optionsArray;
		int menuLevel;
        int currentOption;
		// DX11
		inline static UINT VPS = 1;
		Vector2 ScreenSize = { 0, 0 };
		Vector2 ScreenCenter = { 0, 0 };
		D3D11_VIEWPORT Viewport;
        private:
	};
	inline std::unique_ptr<renderer> g_Renderer;
}