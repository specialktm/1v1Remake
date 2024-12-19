#include "../renderer.hpp"
#include "submenu/Submenu.h"

namespace cheat
{

	void Submenus::Home()
	{
		Title("Home");
		NewSub(ICON_FA_COGS" Settings", Menus::MenuSettings);
	
	
	}

}