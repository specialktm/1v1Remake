#include "../renderer.hpp"
#include "submenu/Submenu.h"

namespace cheat
{

	void Submenus::Home()
	{
		Title("Home");

		NewSub("Settings", Menus::MenuSettings);
	}

}