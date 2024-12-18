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
		void OnPresent();
		void Style();
	public:
		bool m_Open = true;
		C_ImMMenu Menu;


		// Submenus
        void* currentMenu = Submenus::Home;;
        std::vector<void*> menusArray;
        std::vector<int> optionsArray;
		int menuLevel;
        int currentOption;

        private:
	};
	inline std::unique_ptr<renderer> g_Renderer;
}