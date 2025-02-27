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
	public:
		bool m_RichPresenceEnabled = true;
		static const char* m_DiscordUsername;
	private:
		static void HandlerDiscordReady(const DiscordUser* ConnectedUser);
		static void HandlerDiscordDisconnected(int ErrorCode, const char* ErrorMessage);
		static void HandlerDiscordError(int ErrorCode, const char* ErrorMessage);
	private:
		const char* m_LargeImage = "https://isniffsharpie.com/Icons/7rUfmwF.png";
		const char* m_SmallImage = "https://isniffsharpie.com/Icons/hack_bird.gif";
	};
	inline DiscordManager g_DiscordManager;
}