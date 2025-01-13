#include "../renderer.hpp"
#include "submenu/Submenu.h"
#include "Sound/SoundLoader.h"

namespace cheat
{

	void Submenus::Home()
	{
		Title("Home");
		NewSub("Players", Menus::Players);
		NewSub("ESP", Menus::MenuESP);
		NewSub("Settings", Menus::MenuSettings);
	
		
	}
}