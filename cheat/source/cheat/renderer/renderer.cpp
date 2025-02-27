#include "renderer.hpp"
#include "Menu/Submenu/Submenu.h"
#include "../Features/features.h"
#include "../discord/discord.h"

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

			ImGui::Text("%s | Date: %s", APP_NAME, date.data());
			ImGui::Text("Connected Discord User %s", g_DiscordManager.m_DiscordUsername);

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

		if (GetAsyncKeyState(VK_INSERT) & 0x8000) 
		{
			m_Open ^= 1;
		}


        // Render Loops
		if (m_ShouldRendererWatermark) RenderWatermark();

		if (features::EnableEsp) ESP();

    
        if (m_Open && g_Running)
        { 
            if (Menu.Begin())
            {
                Menu.SetHeaderText(APP_NAME);
                Submenus::Tick();
                Menu.SetFooterText(APP_VERSION);
                Menu.End();
            }
         
        }
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
