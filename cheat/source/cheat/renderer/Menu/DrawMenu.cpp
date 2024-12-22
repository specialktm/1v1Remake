#include "../renderer.hpp"
#include "submenu/Submenu.h"

namespace cheat
{

	void Submenus::Home()
	{
		Title("Home");
		NewSub("ESP", Menus::MenuESP);
		NewSub("Settings", Menus::MenuSettings);
	}

}