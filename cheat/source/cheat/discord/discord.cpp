#include "discord.h"
#include "../hooking/hooking.h"

namespace cheat
{

	void DiscordManager::HandlerDiscordReady(const DiscordUser* ConnectedUser)
	{
		m_DiscordUser.username = ConnectedUser->username;
		m_DiscordUser.userId = ConnectedUser->userId;
		m_DiscordUser.avatar = ConnectedUser->avatar;
		m_DiscordUser.discriminator = ConnectedUser->discriminator;
		
		g_logger->send(levels::info, "Discord User Connected: {} UserId: {}", m_DiscordUser.username, m_DiscordUser.userId);
		g_logger->send(levels::info, "Discord User Avatar: https://cdn.discordapp.com/avatars/{}/{}.webp?size=80", m_DiscordUser.userId, m_DiscordUser.avatar);

		m_SmallImage = std::format("https://discordimageproxy.isniffsharpie.com/avatars/{}/{}.webp?size=80", m_DiscordUser.userId, m_DiscordUser.avatar).c_str();
	}

	void DiscordManager::HandlerDiscordDisconnected(int ErrorCode, const char* ErrorMessage)
	{
		g_logger->send(levels::error, "Discord Disconnected: Error Code: {}; Error Message: {};", ErrorCode, ErrorMessage);
	}

	void DiscordManager::HandlerDiscordError(int ErrorCode, const char* ErrorMessage)
	{
		g_logger->send(levels::critical, "Discord Error: Error Code: {}; Error Message: {};", ErrorCode, ErrorMessage);
	}
	inline long long m_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	void DiscordManager::Initialize(std::string_view ApplicationId)
	{
		DiscordEventHandlers m_DiscordEventHandlers{};
		memset(&m_DiscordEventHandlers, 0, sizeof(m_DiscordEventHandlers));

		m_DiscordEventHandlers.ready = [](const DiscordUser* ConnectedUser) 
		{ g_DiscordManager.HandlerDiscordReady(ConnectedUser); };

		m_DiscordEventHandlers.disconnected = [](int ErrorCode, const char* ErrorMessage)
		{ g_DiscordManager.HandlerDiscordDisconnected(ErrorCode, ErrorMessage); };

		m_DiscordEventHandlers.errored = [](int ErrorCode, const char* ErrorMessage)
		{ g_DiscordManager.HandlerDiscordError(ErrorCode, ErrorMessage); };

		Discord_Initialize(ApplicationId.data(), &m_DiscordEventHandlers, 1, 0);
#ifdef Developer
		//m_SmallImage = "https://isniffsharpie.com/Icons/Developer.gif";
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


		DiscordRichPresence m_Presence{};
		memset(&m_Presence, 0, sizeof(m_Presence));
		m_Presence.state = "Playing With " APP_NAME " Menu";
		m_Presence.details = "Using Version " APP_VERSION;
		m_Presence.largeImageKey = m_LargeImage;
		m_Presence.smallImageKey = m_SmallImage;
		m_Presence.smallImageText = m_DiscordUser.username;
		Discord_UpdatePresence(&m_Presence);

	}
	void DiscordManager::UnInitialize()
	{
		g_logger->send(levels::debug, "UnInitializing Discord Manager");
		Discord_ClearPresence();
		Discord_Shutdown();
	}
}