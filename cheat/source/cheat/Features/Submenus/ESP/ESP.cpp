#include "../../../renderer/Menu/Submenu/Submenu.h"
#include "../../features.h"

namespace cheat
{
	void Submenus::Menus::MenuESP()
	{
		Title("ESP");
		Bool("Skeleton Esp", "Draw lines on the player skeleton", &features::SkeletonEsp);
		Bool("Box Esp", "Draw box around the player", &features::BoxEsp);
	}
}