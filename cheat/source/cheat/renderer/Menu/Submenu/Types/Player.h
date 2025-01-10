#pragma once

#include "../Submenu.h"
namespace cheat
{
	struct t_playersubmenu
	{
		std::string name{};
		std::string uuid{};
		std::string skin{};

	};

	inline std::vector<t_playersubmenu> playerMenu{};
	inline int PlayerIndex{};

	namespace Submenus
	{
		void PlayerSubmenu(t_playersubmenu player);
	}
}