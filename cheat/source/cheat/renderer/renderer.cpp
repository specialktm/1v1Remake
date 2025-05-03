#include "renderer.hpp"
#include "Menu/Submenu/Submenu.h"
#include "../Features/features.h"
#include "../discord/discord.h"
#include "notification/notification.h"
#include "Menu/InputHandler/InputHandler.hpp"
namespace cheat
{

    renderer::~renderer()
    {
        currentMenu = nullptr;
        menusArray.clear();
        optionsArray.clear();

		SetWindowLongPtrA(D3D11::m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::ogWndProc));
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
    }

	void renderer::RenderWatermark()
	{
		ImGui::SetNextWindowPos(ImVec2(D3D11::m_WindowRect.right - ScreenSize.x, 7.f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImColor(255, 255, 255).Value);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(0, 0, 0, 255).Value);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.f);
		if (ImGui::Begin("##Watermark", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar))
		{
			time_t DOT = system_clock::to_time_t(system_clock::now());
			std::string_view date = std::ctime(&DOT);

			ImGui::PushFont(Menu.Font.Primary);
			ImGui::Text("%s: Discord: %s", APP_NAME,g_DiscordManager.m_DiscordUser.username);
			ImGui::PopFont();
			ImGui::SameLine();

			ImGui::PushFont(Menu.Font.FontAwesome);
			ImGui::Text(ICON_FA_CLOCK);
			ImGui::PopFont();
			ImGui::SameLine();
			
			ImGui::PushFont(Menu.Font.Primary);
			ImGui::Text("%s", date.data());
			ImGui::PopFont();
		}
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
		ImGui::End();
	
	}

    void renderer::OnPresent()
    {
		if (!g_Running)
			return;

		
		if (MainMenuKeyboard.IsPressed() or MainMenuController.IsPressed()) 
		{
			m_Open ^= 1; 
		}


        // Render Loops

	//g_Interface.RenderInterface();

        if (m_Open && g_Running)
        { 
			if (m_ListInterface)
			{
				if (Menu.Begin())
				{
					Menu.SetHeaderText(APP_NAME);
					Submenus::Tick();
					Menu.SetFooterText(APP_VERSION);
					Menu.End();
				}
			}
			else
			{
				ImGui::SetNextWindowSize({ 550,320 });
				ImGui::PushFont(Menu.Font.Primary);
				if (ImGui::Begin(APP_NAME " " APP_VERSION, &g_Running,
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_AlwaysAutoResize |
					ImGuiWindowFlags_AlwaysUseWindowPadding))
				{
					this->RenderClickInterface();
				}
				ImGui::PopFont();
				ImGui::End();
			}
        }
		if (m_ShouldRendererWatermark) RenderWatermark();
		if (features::EnableEsp) ESP();

		ImGui::render_notifications();

    
    }

	float renderer::Text(bool foreground, ImFont* pFont, const ImVec2& pos, float size, ImU32 color, bool center, bool outlined, const char* text, ...)
	{
		va_list(args);
		va_start(args, text);

		CHAR wbuffer[256] = { };
		vsprintf_s(wbuffer, text, args);

		va_end(args);

		auto DrawList = foreground ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, wbuffer);

			if (center)
			{
				DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}
			else if (outlined)
			{
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), wbuffer);

				DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}
			else
			{
				DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), wbuffer);
			}
			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}

}
