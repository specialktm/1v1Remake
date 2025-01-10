#include "../../../renderer/Menu/Submenu/Submenu.h"


namespace cheat
{

	void Submenus::Menus::Players()
	{
		if (playerMenu.empty()) return;

		for (std::size_t p = 0; p < playerMenu.size(); p++)
		{
			if (p >= playerMenu.size()) continue;

			const std::string& playerName = playerMenu[p].name;
			if (playerName.empty()) return;

			Button(playerName.c_str(), [p]()
			{
				PlayerIndex = p;
				Move(SelectedPlayer);
			});
		}
	}



	void Submenus::Menus::SelectedPlayer()
	{
		PlayerSubmenu(playerMenu[PlayerIndex]);
	}
}