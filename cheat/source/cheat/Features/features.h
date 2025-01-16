#pragma once
#include "../renderer/renderer.hpp"


namespace cheat::features
{
	inline bool SkeletonEsp;
	inline bool BoxEsp;
	inline bool Aimbot;
	bool ExecAimbot(Unity::CGameObject* target, Vector2 playerHead);

	inline Unity::CGameObject* LastPlayer;
}

