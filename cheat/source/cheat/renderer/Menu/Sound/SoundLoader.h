#pragma once
#include "../../renderer.hpp"
#include <audio.h>

namespace cheat
{

	class SoundLoader
	{
		// More Soon
	public:
		bool PlaySound(fs::path& file);
	};
	inline SoundLoader g_SoundLoader;
}
