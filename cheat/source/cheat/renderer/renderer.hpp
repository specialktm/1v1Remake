#pragma once
#include "../../pch/pch.h"
#include "Menu/Menu.hpp"
#include "../../util/logger/logger.hpp"
#include "ThemeLoader/ThemeLoader.h"
#include "Menu/Submenu/Submenu.h"
#include "InterfaceRewrite/Interface.hpp"
namespace cheat
{

	namespace Submenus
	{
		void Home();
	}

	class renderer 
	{
	public:
		~renderer();
		void OnPresent();
		void RenderClickInterface();
        float Text(bool foreground, ImFont* pFont, const ImVec2& pos, float size, ImU32 color, bool center, bool outlined, const char* text, ...);
        void ESP();
        void RenderWatermark();
        void SetInterface(bool pList = true)
        {
			m_ListInterface = pList;
        }
	public:
		bool m_Open = true;
		bool m_ListInterface = true;
		bool m_ShouldRendererWatermark = true;
		C_ImMMenu Menu;
		std::unique_ptr<Interface> Interface;
		// Submenus
        void* currentMenu = Submenus::Home;
        std::vector<void*> menusArray;
        std::vector<int> optionsArray;
		int menuLevel;
        int currentOption;
		// DX11
		Vector2 ScreenSize = { 0, 0 };
		Vector2 ScreenCenter = { 0, 0 };
		D3D11_VIEWPORT Viewport;
        private:
		std::chrono::steady_clock::time_point m_LastToggleTime = std::chrono::steady_clock::now();
	};
	inline std::unique_ptr<renderer> g_Renderer;

	namespace imgui_style
	{
       inline void initStyle()
        {
		   ImGuiStyle* style = &ImGui::GetStyle();

		   style->WindowPadding = ImVec2(15, 15);
		   style->WindowRounding = 5.0f;
		   style->FramePadding = ImVec2(5, 5);
		   style->FrameRounding = 4.0f;
		   style->ItemSpacing = ImVec2(12, 8);
		   style->ItemInnerSpacing = ImVec2(8, 6);
		   style->IndentSpacing = 25.0f;
		   style->ScrollbarSize = 10.0f;
		   style->ScrollbarRounding = 9.0f;
		   style->GrabMinSize = 5.0f;
		   style->GrabRounding = 3.0f;

		   // Colors
		   style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // White text
		   style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		   style->Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black window background
		   style->Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black popup background
		   style->Colors[ImGuiCol_Border] = ImVec4(0.64f, 0.00f, 1.00f, 0.88f); // Purple border (outline)
		   style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		   // Frame backgrounds (black for active)
		   style->Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Dark gray frame background
		   style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Purple hover frame
		   style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black active frame

		   // Title bar (black for active)
		   style->Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black title background
		   style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.64f, 0.00f, 1.00f, 0.75f); // Purple collapsed title background
		   style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black active title background

		   // Menu bar and scrollbar (black for inactive)
		   style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black menu bar
		   style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black scrollbar background
		   style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Dark gray scrollbar grab
		   style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.64f, 0.00f, 1.00f, 0.63f); // Purple scrollbar grab hover
		   style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Active purple scrollbar grab

		   // Buttons, sliders, and checkmarks (dark gray with purple accents)
		   style->Colors[ImGuiCol_CheckMark] = ImVec4(0.64f, 0.00f, 1.00f, 0.31f); // Purple checkmark
		   style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.20f, 0.20f, 0.31f); // Dark gray slider grab
		   style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Active purple slider grab
		   style->Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 0.31f); // Black button background
		   style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Purple hover button
		   style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Active purple button

		   // Tab settings (black for active tabs)
		   style->Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Dark gray tab background
		   style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Purple tab hover
		   style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black active tab

		   style->Colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.20f, 0.86f); // Dark gray tab background
		   style->Colors[ImGuiCol_TabHovered] = ImVec4(0.64f, 0.00f, 1.00f, 0.80f); // Purple tab hover
		   style->Colors[ImGuiCol_TabActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f); // Black active tab
		   style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.20f, 0.97f); // Dark gray unfocused tab
		   style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Active purple unfocused tab

		   // Other elements (like separators, etc.)
		   style->Colors[ImGuiCol_PlotLines] = ImVec4(0.64f, 0.00f, 1.00f, 0.63f); // Purple plot lines
		   style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.87f, 0.00f, 1.00f, 1.00f); // Purple plot lines hover
		   style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.64f, 0.00f, 1.00f, 0.63f); // Purple plot histogram
		   style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.00f, 1.00f, 1.00f); // Purple plot histogram hover
		   style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.64f, 0.00f, 1.00f, 0.43f); // Purple selection background
		   style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.64f, 0.00f, 1.00f, 0.78f); // Purple separator hover
		   style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f); // Active purple separator
        }
	}
}