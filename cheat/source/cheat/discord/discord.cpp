#include "discord.h"

namespace cheat
{



	void DiscordManager::HandlerDiscordReady(const DiscordUser* ConnectedUser)
	{
		g_logger->send(levels::info, "Discord User Connected: {}:{}", ConnectedUser->username, ConnectedUser->userId);
	}

	void DiscordManager::HandlerDiscordDisconnected(int ErrorCode, const char* ErrorMessage)
	{
		g_logger->send(levels::error, "Discord Disconnected: Error Code: {}; Error Message: {};", ErrorCode, ErrorMessage);
	}

	void DiscordManager::HandlerDiscordError(int ErrorCode, const char* ErrorMessage)
	{
		g_logger->send(levels::error, "Discord Error: Error Code: {}; Error Message: {};", ErrorCode, ErrorMessage);
	}

	void DiscordManager::Initialize(std::string_view ApplicationId)
	{
		DiscordEventHandlers m_DiscordEventHandlers{};
		memset(&m_DiscordEventHandlers, 0, sizeof(m_DiscordEventHandlers));
		m_DiscordEventHandlers.ready = HandlerDiscordReady;
		m_DiscordEventHandlers.disconnected = HandlerDiscordDisconnected;
		m_DiscordEventHandlers.errored = HandlerDiscordError;

		Discord_Initialize(ApplicationId.data(), &m_DiscordEventHandlers, 1, 0);
#ifdef Developer
		m_SmallImage = "https://isniffsharpie.com/Icons/Developer.gif";
#endif
	}

	void DiscordManager::Tick()
	{
		if (!m_RichPresenceEnabled) 
		{
			Discord_ClearPresence();
			return;
		}
#ifdef DISCORD_DISABLE_IO_THREAD
		Discord_UpdateConnection();
#endif
		Discord_RunCallbacks();

		DiscordRichPresence m_Presence;
		memset(&m_Presence, 0, sizeof(m_Presence));
		
		
		m_Presence.state = "Playing With " APP_NAME " Menu";
		m_Presence.details = "Using Version " APP_VERSION;
		m_Presence.largeImageKey = m_LargeImage;
		m_Presence.smallImageKey = m_SmallImage;
		Discord_UpdatePresence(&m_Presence);

	}
	void DiscordManager::UnInitialize()
	{
		g_logger->send(levels::debug, "UnInitializing Discord Manager");
		Discord_ClearPresence();
		Discord_Shutdown();
	}
}