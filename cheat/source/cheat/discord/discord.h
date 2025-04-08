#include "../../pch/pch.h"
#include <discord_rpc.h>
#include <discord_register.h>
#include "../../util/logger/logger.hpp"

namespace cheat
{
	class DiscordManager
	{
	public:
		void Initialize(std::string_view ApplicationId);
		void Tick();
		void UnInitialize();
	public: // Useless I know
		bool m_RichPresenceEnabled = true;
		DiscordUser m_DiscordUser{};
	private:
		void HandlerDiscordReady(const DiscordUser* ConnectedUser);
		void HandlerDiscordDisconnected(int ErrorCode, const char* ErrorMessage);
		void HandlerDiscordError(int ErrorCode, const char* ErrorMessage);
	private:
		std::string_view m_LargeImage = "https://isniffsharpie.com/Icons/7rUfmwF.png";
		std::string_view m_SmallImage = "https://isniffsharpie.com/Icons/hack_bird.gif";
	};
	inline DiscordManager g_DiscordManager;
}