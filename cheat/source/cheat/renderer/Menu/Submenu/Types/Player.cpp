#include "Player.h"

namespace cheat
{
	void Submenus::PlayerSubmenu(t_playersubmenu player)
	{
		Button(player.name.c_str(), [] {}, player.name.c_str());
		Button(player.uuid.c_str(), [] {}, player.uuid.c_str());
		Button(player.skin.c_str(), [] {}, player.skin.c_str());
		Button(std::format("Rank XP: {}", player.RankXP).c_str(), [] {}, std::to_string(player.RankXP).c_str());
		Button(std::format("Kill Count: {}", player.KillCount).c_str(), [] {}, std::to_string(player.KillCount).c_str());
	}
}