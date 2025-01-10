#pragma once
#include "../../../pch/pch.h"
#include <math.h>
#include <string>
#include <vector>

#define IMMENU_INLINE	__forceinline

#define IMMENU_TEXT_CENTER_VERTICAL 0.375f // You might need to change this later, but the font that is used by default has weird padding on top...

#define IMMENU_ITEM_NAME_SCROLL_SPEED 100.0f
#define IMMENU_ITEM_NAME_SCROLL_WAIT_TIME 3.0

#include "Fonts/Primary.hpp"
#include "Fonts/Header.hpp"
#include "Fonts/Icons.hpp" // <, =, >

#include "TextMultiColor.hpp"
#include "Items.hpp"
#include "../ImageLoader/ImageLoader.h"
#include "Submenu/Submenu.h"
#include "Fonts/font_awesome_data.cpp"
#undef min

#define IMMENU_ICON_LEFTARROW			"<"
#define IMMENU_ICON_RIGHTARROW			">"
#define IMMENU_ICON_UPDOWNARROW			"="



inline float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

namespace Submenus
{
	void Move(void* Submenu);
	void Back();
}


namespace cheat
{

	class C_ImMMenu
	{
	public:
		//==========================================================================
		// Members

		ImVec2 m_Pos;
		float m_FrameWidth;
		double m_CurrentTime = 0.0;
		ImDrawList* m_DrawList;
		ImVec2 m_DrawPos;
		float m_CurrentCoord;
		float m_CurrentCoord2;

		//==========================================================================
		// Header

		struct Header_t
		{
			float m_Height;
			C_ImMMenuTextMultiColor m_Text;
			ImVec2 m_ImageSize;
			int m_HeaderFrame = 0;
			std::map<int, FrameData> m_Header;

			float m_Rounding = 4.f;
			bool m_Rounded = true;
		};
		Header_t Header;


		__inline void SetHeaderText(std::string m_Text) { Header.m_Text.Initialize(m_Text); }

		//==========================================================================
		// Title Text

		C_ImMMenuTextMultiColor m_TitleText;
		__inline void SetTitleText(std::string m_Text)
		{
			m_TitleText.Initialize(m_Text);
		}

		//==========================================================================
		// Footer

		C_ImMMenuTextMultiColor m_FooterText;
		__inline void SetFooterText(std::string m_Text)
		{
			m_FooterText.Initialize(m_Text);
		}

		//==========================================================================
		// Item

		struct Item_t
		{
			std::vector<C_ImMMenuItem*> m_List;
			__inline int GetCount() {
				return static_cast<int>(m_List.size());
			}
			__inline C_ImMMenuItem* Get(int p_Index) {
				return m_List[p_Index];
			}

			std::vector<int> m_SelectableList;
			__inline int GetSelectableCount() {
				return static_cast<int>(m_SelectableList.size());
			}
			__inline int GetSelectable(int p_Index) {
				return m_SelectableList[p_Index];
			}
			__inline C_ImMMenuItem* GetSelectableItem(int p_Index) {
				return Get(m_SelectableList[p_Index]);
			}

			void CleanUp()
			{
				for (int i = 0; GetCount() > i; ++i)
				{
					C_ImMMenuItem* m_Item = Get(i);
					if (m_Item)
						delete m_Item;
				}

				m_List.clear();
				m_SelectableList.clear();
			}


			__inline bool IsSelectable(eImMMenuItemType m_Type)
			{
				if (m_Type == eImMMenuItemType_Unknown || m_Type == eImMMenuItemType_Separator || m_Type == eImMMenuItemType_TextUnselectable)
					return false;

				return true;
			}

			void AddDummy(eImMMenuItemType m_Type)
			{
				m_List.emplace_back(nullptr);
				if (IsSelectable(m_Type))
					m_SelectableList.emplace_back(GetCount() - 1);
			}

			int AddNewItem(C_ImMMenuItem* m_Item)
			{
				m_List.emplace_back(m_Item);
				if (IsSelectable(m_Item->m_Type))
				{
					;
					m_SelectableList.emplace_back(GetCount() - 1);
					return (GetSelectableCount() - 1);
				}

				return -2;
			}

			int m_NumToShow = 10;
			int m_Index = 0; // From which item to start render
			int m_Selected = 0; // Current selected item
			int m_Interacted = -1; // If there was value change or pressed this value will be temporary set

			void ResetSelection()
			{
				m_Index = 0;
				m_Selected = 0;
				m_Interacted = -1;
			}

			__inline bool IsSelectedValid()
			{
				int m_Count = GetSelectableCount();
				if (0 > m_Selected || m_Selected >= m_Count)
					return false;

				return true;
			}

			__inline bool IsAnyInteracted() { return (m_Interacted != -1); }
			__inline C_ImMMenuItem* GetInteracted()
			{
				if (!IsAnyInteracted())
					return nullptr;

				return GetSelectableItem(m_Interacted);
			}

			void UpdateSideInteraction(int m_Value)
			{
				C_ImMMenuItem* m_Item = GetSelectableItem(m_Selected);
				if (m_Item)
					m_Item->SideInteraction(m_Value);
			}

			void UpdateInteraction()
			{
				C_ImMMenuItem* m_Item = GetInteracted();
				if (m_Item)
					m_Item->Interaction();
			}

			bool IsDummy(eImMMenuItemType m_Type)
			{
				int m_Count = GetCount();
				if (m_Count >= m_Index && (m_Index + m_NumToShow) > m_Count)
					return false;

				AddDummy(m_Type);
				return true;
			}

			void AddSeparator(const char* p_Name)
			{
				if (IsDummy(eImMMenuItemType_Separator)) {
					return;
				}

				auto pItem = new C_ImMMenuItemSeparator(p_Name);
				AddNewItem(pItem);
			}

			bool AddText(const char* p_Name, const char* desc = "")
			{
				if (IsDummy(eImMMenuItemType_Text)) {
					return false;
				}

				auto pItem = new C_ImMMenuItem(eImMMenuItemType_Text, p_Name);
				pItem->m_Description = desc;
				return (m_Interacted == AddNewItem(pItem));
			}

			void AddTextUnselectable(const char* p_Name)
			{
				if (IsDummy(eImMMenuItemType_TextUnselectable)) {
					return;
				}

				auto pItem = new C_ImMMenuItem(eImMMenuItemType_TextUnselectable, p_Name);
				AddNewItem(pItem);
			}

			bool AddSection(const char* p_Name)
			{
				if (IsDummy(eImMMenuItemType_Section)) {
					return false;
				}
				
				auto pItem = new C_ImMMenuItem(eImMMenuItemType_Section, p_Name);

				return (m_Interacted == AddNewItem(pItem));
			}


			bool AddCheckbox(const char* p_Name, const char* m_Description, bool* p_Value)
			{
				if (IsDummy(eImMMenuItemType_Checkbox)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemCheckbox(p_Name, p_Value);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddCombo(const char* p_Name, const char* m_Description, int* p_Value, const char** p_Items, int p_NumItems, bool p_Clamp = false)
			{
				if (IsDummy(eImMMenuItemType_Combo)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemCombo(p_Name, p_Value, p_Items, p_NumItems, p_Clamp);
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddCombo(const char* p_Name, const char* m_Description, int* p_Value, std::vector<std::string>& p_Items, bool p_Clamp = false)
			{
				if (IsDummy(eImMMenuItemType_Combo)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemCombo(p_Name, p_Value, p_Items, p_Clamp);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddComboCheckbox(const char* p_Name, const char* m_Description, int* p_Value, std::vector<bool>* p_Values, const char** p_Items, int p_NumItems, bool p_Clamp = false)
			{
				if (IsDummy(eImMMenuItemType_ComboCheckbox)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemComboCheckbox(p_Name, p_Value, p_Values, p_Items, p_NumItems, p_Clamp);
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddComboCheckbox(const char* p_Name, const char* m_Description, int* p_Value, std::vector<bool>* p_Values, std::vector<std::string>& p_Items, bool p_Clamp = false)
			{
				if (IsDummy(eImMMenuItemType_ComboCheckbox)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemComboCheckbox(p_Name, p_Value, p_Values, p_Items, p_Clamp);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddInteger(const char* m_Name, const char* m_Description, int* p_Value, int m_Min, int m_Max, int m_Power = 1, bool m_Clamp = false)
			{
				if (IsDummy(eImMMenuItemType_Integer))
					return false;

				auto pItem = new C_ImMMenuItemInteger(m_Name, p_Value, m_Min, m_Max, m_Power, m_Clamp);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddFloat(const char* m_Name, const char* m_Description, float* p_Value, float m_Min, float m_Max, float m_Power = 0.1f, const char* m_PreviewFormat = "%.3f", bool m_Clamp = false)
			{
				if (IsDummy(eImMMenuItemType_Float)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemFloat(m_Name, p_Value, m_Min, m_Max, m_Power, m_Clamp, m_PreviewFormat);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			bool AddKeybind(const char* m_Name, const char* m_Description, ImGuiKey* p_Value)
			{
				if (IsDummy(eImMMenuItemType_Keybind)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemKeybind(m_Name, p_Value);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			std::string m_TextInputBuffer;
			bool AddTextInput(const char* m_Name, const char* m_Description, std::string m_PopupText, char* m_Buffer, size_t m_BufferSize, ImGuiInputTextFlags m_Flags = 0, bool m_HideBuffer = false)
			{
				if (IsDummy(eImMMenuItemType_InputText)) {
					return false;
				}

				auto pItem = new C_ImMMenuItemInputText(m_Name, m_PopupText, m_Buffer, m_BufferSize, m_Flags, m_HideBuffer);
				pItem->m_Description = m_Description;
				return (m_Interacted == AddNewItem(pItem));
			}

			void SetDescription(std::string m_Description)
			{
				C_ImMMenuItem* m_Item = Get(GetCount() - 1);
				if (m_Item)
					m_Item->m_Description = m_Description;
			}

			std::string GetSelectOfCountString()
			{
				int m_Count = GetSelectableCount();
				return (std::to_string(std::min(m_Count, m_Selected + 1)) + " of " + std::to_string(m_Count));
			}

			void Update(int m_SelectionChange)
			{
				int m_ItemCount = GetCount();
				int m_ItemSelectableCount = GetSelectableCount();

				int m_MaxIndex = (m_ItemCount - m_NumToShow);

				if (m_ItemSelectableCount > 0)
				{
					m_Selected += m_SelectionChange;
					if (0 > m_Selected)
					{
						m_Selected = (m_ItemSelectableCount - 1);
						m_Index = ImClamp(GetSelectable(m_Selected) - m_NumToShow + 1, 0, ImMax(m_MaxIndex, 0));
					}
					else if (m_Selected >= m_ItemSelectableCount)
					{
						m_Selected = 0;
						m_Index = 0;
					}
					else if (m_MaxIndex >= 0)
					{
						int m_SelectedInList = GetSelectable(m_Selected);
						while (1)
						{
							// If selectable is visible in render...
							if (m_SelectedInList >= m_Index && (m_Index + m_NumToShow) > m_SelectedInList)
								break;

							m_Index = (m_Index + m_SelectionChange) % (m_MaxIndex + 1);
						}
					}
				}
				else
				{
					m_Index = 0;
					m_Selected = -1; // We got no item to select, disable selection...
				}
			}

			// Section Handling
			std::vector<int> m_SectionsLast;
			std::vector<std::pair<int, int>> m_SectionLastPos;
			int m_Section = -1;

			__inline int GetSection()
			{
				return m_Section;
			}

			void SetSection(int p_NewSection)
			{
				m_SectionsLast.emplace_back(m_Section);
				m_SectionLastPos.emplace_back(m_Index, m_Selected);

				m_Section = p_NewSection;
				ResetSelection();
			}

			void SetLastSection()
			{
				if (m_SectionsLast.empty()) {
					return;
				}

				ResetSelection();

				auto& m_SectionPos = m_SectionLastPos.back();
				m_Index = m_SectionPos.first;
				m_Selected = m_SectionPos.second;
				m_Section = m_SectionsLast.back();

				m_SectionsLast.pop_back();
				m_SectionLastPos.pop_back();
			}
			std::map<int, FrameData> m_SubtitleImage;
			std::map<int, FrameData> m_BackgroundImage;
			std::map<int, FrameData> m_Image;
			std::map<int, FrameData> m_FooterImage;

			int m_SubtitleFrame = 0;
			int m_BackgroundFrame = 0;
			int m_ScrollerFrame = 0;
			int m_FooterFrame = 0;

			bool m_Rounded = true;
			float m_Rounding = 4.f;

			bool m_FooterRounded = true;
			float m_FooterRounding = 4.f;

			ImVec2 m_BreakUnderline { 125.0f, 0.f };
			//ImVec2 m_BreakUnderline { 165.0f, 0.f };
		};
		Item_t Item;

		// This handles basic scrolling mechanism when item name text is long...
		struct ItemNameScroll_t
		{
			bool m_Direction = false;
			float m_Value = 0.f;
			double m_UpdateTime = 0.0;

			void Reset(double m_CurrentTime, double m_WaitTime)
			{
				m_Direction = false;
				m_Value = 0.f;
				m_UpdateTime = (m_CurrentTime + m_WaitTime);
			}

			void Update(double m_CurrentTime, float m_Min, float m_Max, float m_Speed, double m_WaitTime)
			{
				// Don't update when update time is bigger
				if (m_UpdateTime > m_CurrentTime) {
					return;
				}

				if (m_UpdateTime != 0.0)
				{
					float m_ChangeValue = static_cast<float>(m_CurrentTime - m_UpdateTime) * m_Speed;
					if (m_Direction)
					{
						m_Value += m_ChangeValue;
						if (m_Value > m_Max)
						{
							m_Direction = false;
							m_Value = m_Max;
							m_UpdateTime = (m_CurrentTime + m_WaitTime);
							return;
						}
					}
					else
					{
						m_Value -= m_ChangeValue;
						if (m_Value < m_Min)
						{
							m_Direction = true;
							m_Value = m_Min;
							m_UpdateTime = (m_CurrentTime + m_WaitTime);
							return;
						}
					}
				}

				m_UpdateTime = m_CurrentTime;
			}
		};
		ItemNameScroll_t ItemNameScroll;

		float GetItemNameMaxWidth(eImMMenuItemType p_ItemType)
		{
			switch (p_ItemType)
			{
			case eImMMenuItemType_Text: case eImMMenuItemType_TextUnselectable:
				return (m_FrameWidth - 20.f);
			case eImMMenuItemType_Section: case eImMMenuItemType_Checkbox:
				return floorf(m_FrameWidth * 0.85f);
			default:
				return floorf(m_FrameWidth * 0.75f);
			}
		}

		struct Fonts_t
		{
			ImFont* Icons = nullptr;
			ImFont* Primary = nullptr;
			ImFont* Header = nullptr;
			ImFont* FontAwesome = nullptr;

			bool AllLoaded()
			{
				return (Icons && Primary && Header && FontAwesome);
			}

			__inline ImVec2 CalcTextSize(ImFont* p_Font, const char* p_Text)
			{
				return p_Font->CalcTextSizeA(p_Font->FontSize, FLT_MAX, 0.f, p_Text);
			}
		};
		Fonts_t Font;

		struct Colors_t
		{
			ImU32 Header = IM_COL32(0, 0, 0, 255);
			ImU32 Title = IM_COL32(0, 0, 0, 255);
			ImU32 Footer = IM_COL32(0, 0, 0, 255);
			ImU32 Item = IM_COL32(0, 0, 0, 180);
			ImU32 ItemSelected = IM_COL32(255, 255, 255, 255);
			ImU32 Separator = IM_COL32(255, 255, 255, 255);
			ImU32 Primary = IM_COL32(0, 0, 0, 255);
			ImU32 Description = IM_COL32(0, 0, 0, 200);

			ImU32 Header_Text = IM_COL32(255, 255, 255, 255);
			ImU32 Title_Text = IM_COL32(255, 255, 255, 255);
			ImU32 Footer_Text = IM_COL32(255, 255, 255, 255);
			ImU32 Primary_Text = IM_COL32(255, 255, 255, 255);
			ImU32 Selected_Text = IM_COL32(0, 0, 0, 255);
		};
		Colors_t Color;

		struct Input_t
		{
			char m_SelectUpDown = 0; // -1 = Up | 1 = Down
			__inline void SelectUp() { m_SelectUpDown -= 1; }
			__inline void SelectDown() { m_SelectUpDown += 1; }

			char m_SelectLeftRight = 0; // -1 = Left | 1 = Right
			__inline void SelectLeft() { m_SelectLeftRight -= 1; }
			__inline void SelectRight() { m_SelectLeftRight += 1; }

			bool m_SelectInteraction = false;
			__inline void SelectInteraction() { m_SelectInteraction = true; }

			bool m_EnableNative = true; // You can disable this if you wanna handle it different way...
			double m_NavigationRepeatDelta = 0.25;

			ImGuiKey m_NavigationKeys[4] =
			{
				ImGuiKey_Keypad8, ImGuiKey_Keypad2,
				ImGuiKey_Keypad4, ImGuiKey_Keypad6
			};
			int m_NavigationEmulatedPressMaxCount = 50;
			int m_NagivationEmulatedPressCount[4] = { 0, 0, 0, 0 }; // This is used to make selection faster each time the key is repeated hold and emulated press
			double m_NavigationLastPress[4] = { 0.0, 0.0, 0.0, 0.0 };

			ImGuiKey m_InteractionKey = ImGuiKey_Keypad5;

			bool IsInteractingWithKeybind = false;
			bool IsInteractingWithInputText = false;
			bool BlockedByItem()
			{
				return (IsInteractingWithKeybind || IsInteractingWithInputText);
			}

			void Update(double m_CurrentTime)
			{
				if (BlockedByItem())
				{
					for (int i = 0; 4 > i; ++i)
					{
						m_NagivationEmulatedPressCount[i] = 1;
						m_NavigationLastPress[i] = m_CurrentTime + m_NavigationRepeatDelta;
					}
					return;
				}

				for (int i = 0; 4 > i; ++i)
				{
					if (!ImGui::IsKeyDown(m_NavigationKeys[i]))
					{
						m_NagivationEmulatedPressCount[i] = 1;
						m_NavigationLastPress[i] = 0.0;
						continue;
					}

					if (m_NavigationLastPress[i] > m_CurrentTime) {
						continue;
					}

					if (m_NavigationEmulatedPressMaxCount > m_NagivationEmulatedPressCount[i]) {
						++m_NagivationEmulatedPressCount[i];
					}

					m_NavigationLastPress[i] = (m_CurrentTime + (m_NavigationRepeatDelta / (static_cast<double>(m_NagivationEmulatedPressCount[i]) * 0.5)));

					switch (i)
					{
					case 0: SelectUp(); break;
					case 1: SelectDown(); break;
					case 2: SelectLeft(); break;
					case 3: SelectRight(); break;
					}
				}

				if (ImGui::IsKeyPressed(m_InteractionKey, false))
					SelectInteraction();
			}

			ImGuiKey GetAnyPressed()
			{
				for (int i = ImGuiKey_NamedKey_BEGIN; ImGuiKey_NamedKey_END > i; ++i)
				{
					ImGuiKey m_Key = static_cast<ImGuiKey>(i);
					if (ImGui::IsKeyPressed(m_Key, false))
						return m_Key;
				}

				return ImGuiKey_None;
			}

			bool m_CaptureMouse = false;
		};
		Input_t Input;

		// Call this after ImGui_ImplWin32_Init & ImGui_[BackEnd_Renderer]_Init...
		bool Initialize()
		{
			ImGuiIO* m_IO = &ImGui::GetIO();
			// Fonts
			Font.Icons = m_IO->Fonts->AddFontFromMemoryCompressedTTF(ImGuiMMenu::Font::Icons_Data, ImGuiMMenu::Font::Icons_SizeData, 16.f);
			Font.Primary = m_IO->Fonts->AddFontFromMemoryCompressedTTF(ImGuiMMenu::Font::Primary_Data, ImGuiMMenu::Font::Primary_SizeData, 16.f);
			Font.Header = m_IO->Fonts->AddFontFromMemoryCompressedTTF(ImGuiMMenu::Font::Header_Data, ImGuiMMenu::Font::Header_SizeData, 64.f);
			
			ImFontConfig IcoConfig;
			IcoConfig.MergeMode = true;
			IcoConfig.PixelSnapH = true;
			IcoConfig.OversampleH = 3;
			IcoConfig.OversampleV = 3;
			static const ImWchar IcoRanges[]{ 0xf000,0xf3ff,0 };

			Font.FontAwesome = m_IO->Fonts->AddFontFromMemoryCompressedTTF
			(
				font_awesome_data, 
				font_awesome_size, 
				19.f, 
				&IcoConfig, 
				IcoRanges
			);
			
			if (!Font.AllLoaded()) {
				return false;
			}

			// Sizes
			m_Pos = ImVec2(0.05f, 0.05f);
			m_FrameWidth = 420.f;

			Header.m_Height = 100.f;

			return true;
		}

		bool Begin()
		{
			ImGuiIO* m_IO = &ImGui::GetIO();

			m_CurrentTime = ImGui::GetTime();
			m_DrawList = ImGui::GetBackgroundDrawList();
			m_DrawPos = { ImMin(m_IO->DisplaySize.x - m_FrameWidth, floorf(m_Pos.x * m_IO->DisplaySize.x)), floorf(m_Pos.y * m_IO->DisplaySize.y) };

			if (Input.m_EnableNative) {
				Input.Update(m_CurrentTime);
			}

			//Input.m_CaptureMouse = (!m_IO->WantCaptureMouse);

			return true;
		}
		
		int last_frame_count_header = 0;
		int last_frame_count_subtitle = 0;
		void End()
		{
			// Header
			{


				if (!Header.m_Header.empty()) {
					static timer::simpleTimer tick(0ms);
					tick.set_delay(Header.m_Header[Header.m_HeaderFrame].m_Delay);
					if (tick.update() && last_frame_count_header != ImGui::GetFrameCount())
					{
						last_frame_count_header = ImGui::GetFrameCount();
						if (Header.m_Header.size() > 1)
						{
							Header.m_HeaderFrame = static_cast<int>((Header.m_HeaderFrame + 1) % Header.m_Header.size());
						}

					}
						m_DrawList->AddImageRounded
						(
							Header.m_Header[Header.m_HeaderFrame].m_Texture, 
							m_DrawPos, 
							m_DrawPos + ImVec2(m_FrameWidth, Header.m_Height), 
							ImVec2(0, 0), 
							ImVec2(1, 1), 
							IM_COL32_WHITE, 
							Header.m_Rounded ? Header.m_Rounding : 0,
							Header.m_Rounded ? ImDrawFlags_RoundCornersTop : 0
						);
					
				}
				else
				{
					m_DrawList->AddRectFilled(
						m_DrawPos, 
						m_DrawPos + ImVec2(m_FrameWidth, Header.m_Height), 
						Color.Header, 
						Header.m_Rounded ? Header.m_Rounding : 0,
						Header.m_Rounded ? ImDrawFlags_RoundCornersTop : 0
					);
				}
				if (Header.m_Text.GetCount() && Header.m_Header.empty())
				{
					ImVec2 vTextSize = Header.m_Text.CalcTextSize(Font.Header);
					ImVec2 vTextPos(m_DrawPos + ImVec2(ImFloor((m_FrameWidth * 0.5f) - (vTextSize.x * 0.5f)), ImFloor((Header.m_Height * 0.5f) - (vTextSize.y * IMMENU_TEXT_CENTER_VERTICAL))));
					Header.m_Text.Draw(m_DrawList, Font.Header, Font.Header->FontSize, vTextPos);
				}

				m_DrawPos.y += Header.m_Height;

			}



				
			// Title
			if (m_TitleText.GetCount())
			{
				static timer::simpleTimer tick(0ms);

				if (!Item.m_SubtitleImage.empty())
				{
					tick.set_delay(Item.m_SubtitleImage[Item.m_SubtitleFrame].m_Delay);

					if (tick.update() && last_frame_count_subtitle != ImGui::GetFrameCount())
					{
						last_frame_count_subtitle = ImGui::GetFrameCount();
						if (Item.m_SubtitleImage.size() > 1)
						{
							Item.m_SubtitleFrame = static_cast<int>((Item.m_SubtitleFrame + 1) % Item.m_SubtitleImage.size());
						}
					}
				}


				float fTitleHeight = ImFloor(Font.Primary->FontSize * 2.f);
				if (Item.m_SubtitleImage.empty())
				{
					m_DrawList->AddRectFilled(
						m_DrawPos,
						m_DrawPos + ImVec2(m_FrameWidth, fTitleHeight),
						Color.Title
					);
				}
				else
				{
					if (Item.m_SubtitleFrame >= 0 && Item.m_SubtitleFrame < Item.m_SubtitleImage.size())
					{
						m_DrawList->AddImage(
							Item.m_SubtitleImage[Item.m_SubtitleFrame].m_Texture,
							m_DrawPos,
							m_DrawPos + ImVec2(m_FrameWidth, fTitleHeight),
							ImVec2(0, 0),
							ImVec2(1, 1),
							IM_COL32_WHITE
						);
					}
					else
					{
					//	g_logger->send(levels::error, "Invalid subtitle frame index: {}", Item.m_SubtitleFrame);
						m_DrawList->AddRectFilled(
							m_DrawPos,
							m_DrawPos + ImVec2(m_FrameWidth, fTitleHeight),
							Color.Title
						);
					}
				}

				ImVec2 vTextSize = m_TitleText.CalcTextSize(Font.Primary);
				ImVec2 vTextPos(m_DrawPos + ImVec2(ImFloor((m_FrameWidth * 0.5f) - (vTextSize.x * 0.5f)), ImFloor((fTitleHeight * 0.5f) - (vTextSize.y * IMMENU_TEXT_CENTER_VERTICAL))));
				m_TitleText.Draw(m_DrawList, Font.Primary, Font.Primary->FontSize, vTextPos);

				m_DrawPos.y += fTitleHeight;
			}

			// Items
			int last_scroller_count = 0;
			int last_background_count = 0;
			int last_frame_count_footer = 0;
			if (Item.GetCount())
			{

				// Reset index & selectable when selectable is above count
				// Probably would need add hash item lookup to reset selectable position...
				int m_SelectableCount = Item.GetSelectableCount();
				if (Item.m_Selected >= m_SelectableCount)
				{
					Item.m_Index = 0;
					Item.m_Selected = (m_SelectableCount ? 0 : -1);
				}

				int firstIndex = Item.m_Index;
				int lastIndex = ImMin(Item.GetCount(), Item.m_Index + Item.m_NumToShow);
				float itemHeight = 0.0f;
				for (int i = firstIndex; i < lastIndex; ++i) {
					C_ImMMenuItem* pItem = Item.Get(i);
					if (pItem) {
						itemHeight += floorf(pItem->GetName().CalcTextSize(Font.Primary).y * 2.f);
					}
				}


				ImVec2 size(m_FrameWidth, itemHeight); // gayyy
				if (Item.m_BackgroundImage.empty())
				{
					m_DrawList->AddRectFilled(
						m_DrawPos,
						m_DrawPos + size,
						Color.Item
					);
				}
				else {

					static timer::simpleTimer tick(0ms);
					tick.set_delay(Item.m_BackgroundImage[Item.m_BackgroundFrame].m_Delay);
					if (tick.update() && last_background_count != ImGui::GetFrameCount())
					{
						last_background_count = ImGui::GetFrameCount();
						if (Item.m_BackgroundImage.size() > 1)
						{
							Item.m_BackgroundFrame = static_cast<int>((Item.m_BackgroundFrame + 1) % Item.m_BackgroundImage.size());
						}
					}
					m_DrawList->AddImage(
						Item.m_BackgroundImage[Item.m_BackgroundFrame].m_Texture,
						m_DrawPos,
						m_DrawPos + size,
						ImVec2(0, 0),
						ImVec2(1, 1),
						IM_COL32_WHITE
					);
					//printf("Current Frame: %d\n", Functions::m_background_frame);

				}

				// Draw
				for (int i = Item.m_Index; ImMin(Item.GetCount(), Item.m_Index + Item.m_NumToShow) > i; ++i)
				{

					C_ImMMenuItem* pItem = Item.Get(i);
					if (!pItem) {
						continue;
					}

					C_ImMMenuTextMultiColor m_ItemName = pItem->GetName();


					ImVec2 vTextSize = m_ItemName.CalcTextSize(Font.Primary);
					float m_FrameHeight = floorf(vTextSize.y * 2.f);
					ImVec2 vTextPos(m_DrawPos + ImVec2(10.f, floorf((m_FrameHeight * 0.5f) - (vTextSize.y * IMMENU_TEXT_CENTER_VERTICAL))));

					// Pre Left-side Element
					if (pItem->m_Type == eImMMenuItemType_Separator)
					{
						ImVec2 vTextPos(m_DrawPos + ImVec2(floorf((m_FrameWidth * 0.5f) - (vTextSize.x * 0.5f)), floorf((m_FrameHeight * 0.5f) - (vTextSize.y * IMMENU_TEXT_CENTER_VERTICAL))));

						// 125.088,16.000
						//m_DrawList->AddRectFilled(m_DrawPos, m_DrawPos + ImVec2(m_FrameWidth, m_FrameHeight), Color.Item);
						//printf("Text: %s Size:  %.3f,%.3f\n",pItem->m_Name, vTextSize.x, vTextSize.y);
						ImVec2 m_UnderlinePos(m_DrawPos + ImVec2(floorf(m_FrameWidth * 0.5f), floorf((m_FrameHeight * 0.5f) + (vTextSize.y * 0.75f))));
						if (!pItem->m_Name.empty() && pItem->m_Name != "")
						{
							m_ItemName.Draw(ImGui::GetForegroundDrawList(), Font.Primary, Font.Primary->FontSize, vTextPos);
							m_DrawList->AddLine(m_UnderlinePos - ImVec2(floorf(125.f * 1.f) + 20.f, 0.f), m_UnderlinePos + ImVec2(floorf(125.f * 1.0f) + 20.f, 0.f), Color.Separator, 2.f);
							//m_DrawList->AddLine(m_UnderlinePos - ImVec2(floorf(vTextSize.x * 1.f) + 20.f, 0.f), m_UnderlinePos + ImVec2(floorf(vTextSize.x * 1.0f) + 20.f, 0.f), Color.Separator, 2.f);
						}
						else
						{
							m_DrawList->AddLine(vTextPos - ImVec2(floorf(Item.m_BreakUnderline.x * 1.f) + 20.f, 0.f), vTextPos + ImVec2(floorf(Item.m_BreakUnderline.x * 1.0f) + 20.f, 0.f), Color.Separator, 2.f);
						}
						
						m_DrawPos.y += m_FrameHeight;
						continue;
					}

					bool m_Selected = (Item.m_Selected >= 0 && i == Item.GetSelectable(Item.m_Selected));

					if (m_Selected) // draw smooth scrolled selected
					{
						m_CurrentCoord = lerp(m_CurrentCoord, m_DrawPos.y, 0.1f);
						m_CurrentCoord2 = lerp(m_CurrentCoord2, m_DrawPos.x, 0.1f);


						if (Item.m_Image.empty())
						{
							Color.Selected_Text = ImColor(0, 0, 0);
						
							m_DrawList->AddRectFilled(
								ImVec2{ m_CurrentCoord2, m_CurrentCoord },
								ImVec2{ m_CurrentCoord2, m_CurrentCoord } + ImVec2(m_FrameWidth, m_FrameHeight),
								Color.ItemSelected,
								Item.m_Rounded ? Item.m_Rounding : 0,
								Item.m_Rounded ? ImDrawFlags_RoundCornersAll : 0
							);

						}
						else
						{
							static timer::simpleTimer tick(0ms);
							tick.set_delay(Item.m_Image[Item.m_ScrollerFrame].m_Delay);
							if (tick.update() && last_scroller_count != ImGui::GetFrameCount())
							{
								last_scroller_count = ImGui::GetFrameCount();
								if (Item.m_Image.size() > 1)
								{
									Item.m_ScrollerFrame = static_cast<int>((Item.m_ScrollerFrame + 1) % Item.m_Image.size());
								}
							}
							Color.Selected_Text = ImColor(255, 255, 255);
							m_DrawList->AddImageRounded(
								Item.m_Image[Item.m_ScrollerFrame].m_Texture,
								ImVec2{ m_CurrentCoord2, m_CurrentCoord },
								ImVec2{ m_CurrentCoord2, m_CurrentCoord } + ImVec2(m_FrameWidth, m_FrameHeight),
								ImVec2(0, 0),
								ImVec2(1, 1),
								IM_COL32_WHITE,
								Item.m_Rounded ? Item.m_Rounding : 0,
								Item.m_Rounded ? ImDrawFlags_RoundCornersAll : 0
							);
						}
					}

					// This is fine, for now...  
					if (Input.m_CaptureMouse) {
						if (ImGui::IsMouseHoveringRect(m_DrawPos, m_DrawPos + ImVec2(m_FrameWidth, m_FrameHeight), false)) {
							ImVec2& m_MouseDelta = ImGui::GetIO().MouseDelta;
							if (m_MouseDelta.x != 0.f || m_MouseDelta.y != 0.f)
							{
								int m_SelectableItter = Item.GetSelectableCount();
								while (m_SelectableItter)
								{
									--m_SelectableItter;
									int m_SelectableIndex = Item.GetSelectable(m_SelectableItter);
									if (m_SelectableIndex == i) {
										Item.m_Selected = m_SelectableItter;
										break;
									}
								}
							}
							if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
								Input.m_SelectInteraction = true;
							}
						}
					}

					const float m_ItemNameHorizontalMax = GetItemNameMaxWidth(pItem->m_Type);
					C_ImMMenuTextMultiColorClip m_ItemNameClip({ 0.f, 0.f }, { vTextPos.x, vTextPos.y, vTextPos.x + m_ItemNameHorizontalMax, vTextPos.y + vTextSize.y });
					if (m_Selected && vTextSize.x > m_ItemNameHorizontalMax)
					{
						ItemNameScroll.Update(m_CurrentTime, m_ItemNameHorizontalMax - vTextSize.x, 0.f, IMMENU_ITEM_NAME_SCROLL_SPEED, IMMENU_ITEM_NAME_SCROLL_WAIT_TIME);
						m_ItemNameClip.m_Offset.x += ItemNameScroll.m_Value;
					}

					std::string displayedText = &Item.Get(i)->m_Name[0];
					if (vTextSize.x > m_FrameWidth - 20.f) {
						
						displayedText = displayedText.substr(0, displayedText.length() - 20) + "...";
					}

					ImGui::GetForegroundDrawList()->AddText(
						Font.Primary,
						Font.Primary->FontSize,
						vTextPos, 
						m_Selected ? Color.Selected_Text : Color.Primary_Text, 
						displayedText.c_str());
					
					
					
					//ImGui::GetForegroundDrawList()->AddText(Font.Primary, Font.Primary->FontSize, vTextPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, &sString.substr(0, sNewlinePos)[0], nullptr, 0.f, pClip);
					//m_ItemName.Draw(ImGui::GetForegroundDrawList(), Font.Primary, Font.Primary->FontSize, vTextPos, &m_ItemNameClip);

					// Right-side Element
					switch (pItem->m_Type)
					{
					default:
						break;
					case eImMMenuItemType_Section:
					{
						ImVec2 vIconSize = Font.CalcTextSize(Font.Icons, IMMENU_ICON_RIGHTARROW);
						ImVec2 vIconPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - vIconSize.x, floorf((m_FrameHeight * 0.5f) - (vIconSize.y * 0.5f))));

						ImGui::GetForegroundDrawList()->AddText(Font.Icons, Font.Icons->FontSize, vIconPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, IMMENU_ICON_RIGHTARROW);
					}
					break;
					case eImMMenuItemType_Checkbox:
					{
						float m_BoxSize = floorf(m_FrameHeight * 0.25f);
						ImVec2 m_BoxPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - m_BoxSize, floorf(m_FrameHeight * 0.5f)));

						ImGui::GetForegroundDrawList()->AddRect(
							m_BoxPos - ImVec2(m_BoxSize, m_BoxSize),
							m_BoxPos + ImVec2(m_BoxSize, m_BoxSize),
							m_Selected ? Color.Selected_Text : IM_COL32_WHITE,
							2.f,
							ImDrawFlags_RoundCornersAll,
							2.f);

						if (reinterpret_cast<C_ImMMenuItemCheckbox*>(pItem)->IsChecked())
						{
							m_BoxSize -= 2.f;
							ImGui::GetForegroundDrawList()->AddRectFilled(
								m_BoxPos - ImVec2(m_BoxSize, m_BoxSize),
								m_BoxPos + ImVec2(m_BoxSize, m_BoxSize),
								m_Selected ? Color.Selected_Text : IM_COL32_WHITE,
								1.f,
								ImDrawFlags_RoundCornersAll);
						}
					}
					break;
					case eImMMenuItemType_Combo:
					case eImMMenuItemType_ComboCheckbox:
					{
						C_ImMMenuTextMultiColor m_PreviewText = reinterpret_cast<C_ImMMenuItemCombo*>(pItem)->GetPreview();

						ImVec2 vPreviewSize = m_PreviewText.CalcTextSize(Font.Primary);
						ImVec2 vPreviewPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - vPreviewSize.x, floorf((m_FrameHeight * 0.5f) - (vPreviewSize.y * IMMENU_TEXT_CENTER_VERTICAL))));

						if (m_Selected)
						{
							float m_CheckboxOffset = 0.f;
							if (pItem->m_Type == eImMMenuItemType_ComboCheckbox)
							{
								float m_BoxSize = floorf(m_FrameHeight * 0.25f);
								m_CheckboxOffset = (m_BoxSize * 2.f) + 5.f;

								ImVec2 m_BoxPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - m_BoxSize, floorf(m_FrameHeight * 0.5f)));

								ImGui::GetForegroundDrawList()->AddRect(m_BoxPos - ImVec2(m_BoxSize, m_BoxSize), m_BoxPos + ImVec2(m_BoxSize, m_BoxSize), m_Selected ? Color.Selected_Text : IM_COL32_WHITE, 0.f, 0, 2.f);

								if (reinterpret_cast<C_ImMMenuItemComboCheckbox*>(pItem)->IsChecked())
								{
									m_BoxSize -= 2.f;
									ImGui::GetForegroundDrawList()->AddRectFilled(m_BoxPos - ImVec2(m_BoxSize, m_BoxSize), m_BoxPos + ImVec2(m_BoxSize, m_BoxSize), m_Selected ? Color.Selected_Text : IM_COL32_WHITE);
								}
							}

							ImVec2 m_IconRightSize = Font.CalcTextSize(Font.Icons, IMMENU_ICON_RIGHTARROW);
							ImVec2 m_IconRightPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - m_IconRightSize.x - m_CheckboxOffset, floorf((m_FrameHeight * 0.5f) - (m_IconRightSize.y * 0.5f))));
							ImGui::GetForegroundDrawList()->AddText(Font.Icons, Font.Icons->FontSize, m_IconRightPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, IMMENU_ICON_RIGHTARROW);

							vPreviewPos.x = m_IconRightPos.x - vPreviewSize.x - 5.f;

							ImVec2 m_IconLeftSize = Font.CalcTextSize(Font.Icons, IMMENU_ICON_LEFTARROW);
							ImVec2 m_IconLeftPos(m_IconRightPos - ImVec2(vPreviewSize.x + m_IconLeftSize.x + 10.f, 0.f));
							ImGui::GetForegroundDrawList()->AddText(Font.Icons, Font.Icons->FontSize, m_IconLeftPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, IMMENU_ICON_LEFTARROW);
						}

						m_PreviewText.Draw(ImGui::GetForegroundDrawList(), Font.Primary, Font.Primary->FontSize, vPreviewPos);
					}
					break;
					case eImMMenuItemType_Integer:
					case eImMMenuItemType_Float:
					{
						bool m_IsFloat = (pItem->m_Type == eImMMenuItemType_Float);

						std::string m_PreviewText = (m_IsFloat ? reinterpret_cast<C_ImMMenuItemFloat*>(pItem)->GetPreview() : reinterpret_cast<C_ImMMenuItemInteger*>(pItem)->GetPreview());

						ImVec2 vPreviewSize = Font.CalcTextSize(Font.Primary, &m_PreviewText[0]);
						ImVec2 vPreviewPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - vPreviewSize.x, floorf((m_FrameHeight * 0.5f) - (vPreviewSize.y * IMMENU_TEXT_CENTER_VERTICAL))));

						if (m_Selected)
						{
							ImVec2 m_IconRightSize = Font.CalcTextSize(Font.Icons, IMMENU_ICON_RIGHTARROW);
							ImVec2 m_IconRightPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - m_IconRightSize.x, floorf((m_FrameHeight * 0.5f) - (m_IconRightSize.y * 0.5f))));
							ImGui::GetForegroundDrawList()->AddText(Font.Icons, Font.Icons->FontSize, m_IconRightPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, IMMENU_ICON_RIGHTARROW);

							vPreviewPos.x = m_IconRightPos.x - vPreviewSize.x - 5.f;

							ImVec2 m_IconLeftSize = Font.CalcTextSize(Font.Icons, IMMENU_ICON_LEFTARROW);
							ImVec2 m_IconLeftPos(m_IconRightPos - ImVec2(vPreviewSize.x + m_IconLeftSize.x + 10.f, 0.f));
							ImGui::GetForegroundDrawList()->AddText(Font.Icons, Font.Icons->FontSize, m_IconLeftPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, IMMENU_ICON_LEFTARROW);
						}

						ImGui::GetForegroundDrawList()->AddText(Font.Primary, Font.Primary->FontSize, vPreviewPos, m_Selected ? Color.Selected_Text : Color.Primary_Text, &m_PreviewText[0]);
					}
					break;
					case eImMMenuItemType_Keybind:
					{
						ImGuiKey iKey = reinterpret_cast<C_ImMMenuItemKeybind*>(pItem)->GetKey();

						const char* szKeyName;
						{
							if (Item.GetInteracted() == pItem) {
								szKeyName = "Press Key";
							}
							else if (iKey == ImGuiKey_None) {
								szKeyName = "None";
							}
							else {
								szKeyName = ImGui::GetKeyName(iKey);
							}
						}

						const char* szFormat;
						ImFormatStringToTempBuffer(&szFormat, 0, "< %s >", szKeyName);

						ImVec2 vKeySize = Font.CalcTextSize(Font.Primary, szFormat);
						ImVec2 vKeyPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - vKeySize.x, floorf((m_FrameHeight * 0.5f) - (vKeySize.y * IMMENU_TEXT_CENTER_VERTICAL))));

						ImGui::GetForegroundDrawList()->AddText(Font.Primary, Font.Primary->FontSize, vKeyPos, m_Selected ? Color.Selected_Text :
							m_Selected ? Color.Selected_Text : Color.Primary_Text, szFormat);
					}
					break;
					case eImMMenuItemType_InputText:
					{
						if (reinterpret_cast<C_ImMMenuItemInputText*>(pItem)->m_HideBuffer) {
							break;
						}

						std::string m_TextValue = reinterpret_cast<C_ImMMenuItemInputText*>(pItem)->m_Buffer;
						if (m_TextValue.empty())
							break;

						if (m_TextValue.size() > 16)
						{
							m_TextValue.erase(16, m_TextValue.size() - 16);
							m_TextValue += "...";
						}

						ImVec2 m_TextValueSize = Font.CalcTextSize(Font.Primary, &m_TextValue[0]);
						ImVec2 m_TextValuePos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - m_TextValueSize.x, floorf((m_FrameHeight * 0.5f) - (m_TextValueSize.y * IMMENU_TEXT_CENTER_VERTICAL))));

						ImGui::GetForegroundDrawList()->AddText(Font.Primary, Font.Primary->FontSize, m_TextValuePos, m_Selected ? Color.Selected_Text : Color.Primary_Text, &m_TextValue[0]);
					}
					break;
					}

					m_DrawPos.y += m_FrameHeight;
				}
			}

			// Footer
			{
				static timer::simpleTimer tick(0ms);

				// Update the animation frame for the footer image
				if (!Item.m_FooterImage.empty())
				{
					tick.set_delay(Item.m_FooterImage[Item.m_FooterFrame].m_Delay);
					if (tick.update() && last_frame_count_footer != ImGui::GetFrameCount())
					{
						last_frame_count_footer = ImGui::GetFrameCount();
						if (Item.m_FooterImage.size() > 1)
						{
							Item.m_FooterFrame = static_cast<int>((Item.m_FooterFrame + 1) % Item.m_FooterImage.size());
						}
					}
				}

				float m_FooterHeight = floorf(Font.Primary->FontSize * 2.f);
				
				if (Item.m_FooterImage.empty())
				{
					m_DrawList->AddRectFilled(
						m_DrawPos,
						m_DrawPos + ImVec2(m_FrameWidth, m_FooterHeight),
						Color.Footer,
						Item.m_FooterRounded ? Item.m_FooterRounding : 0,
						Item.m_FooterRounded ? ImDrawFlags_RoundCornersBottom : 0
					);
				}
				else
				{
					if (Item.m_FooterFrame >= 0 && Item.m_FooterFrame < Item.m_FooterImage.size())
					{
						m_DrawList->AddImage(
							Item.m_FooterImage[Item.m_FooterFrame].m_Texture,
							m_DrawPos,
							m_DrawPos + ImVec2(m_FrameWidth, m_FooterHeight),
							ImVec2(0, 0),
							ImVec2(1, 1),
							IM_COL32_WHITE
						);
					}
					else
					{
						//g_logger->send(levels::error, "Invalid footer frame index: {}", Item.m_FooterFrame);
						m_DrawList->AddRectFilled(
							m_DrawPos,
							m_DrawPos + ImVec2(m_FrameWidth, m_FooterHeight),
							Color.Footer,
							Item.m_FooterRounded ? Item.m_FooterRounding : 0,
							Item.m_FooterRounded ? ImDrawFlags_RoundCornersBottom : 0
						);
					}
				}

				// Select Info
				{
					std::string m_Text = Item.GetSelectOfCountString();
					ImVec2 vTextSize = Font.CalcTextSize(Font.Primary, &m_Text[0]);
					ImVec2 vTextPos(m_DrawPos + ImVec2(10.f, floorf((m_FooterHeight * 0.5f) - (vTextSize.y * IMMENU_TEXT_CENTER_VERTICAL))));
					m_DrawList->AddText(Font.Primary, Font.Primary->FontSize, vTextPos, Color.Title_Text, &m_Text[0]);
				}

				// Icon Center
				{
					ImVec2 vIconSize = Font.CalcTextSize(Font.Icons, IMMENU_ICON_UPDOWNARROW);
					ImVec2 vIconPos(m_DrawPos + ImVec2(floorf((m_FrameWidth * 0.5f) - (vIconSize.x * 0.5f)), floorf((m_FooterHeight * 0.5f) - (vIconSize.y * 0.5f))));

					m_DrawList->AddText(Font.Icons, Font.Icons->FontSize, vIconPos, Color.Primary_Text, IMMENU_ICON_UPDOWNARROW);
				}

				if (m_FooterText.GetCount())
				{
					ImVec2 vTextSize = m_FooterText.CalcTextSize(Font.Primary);
					ImVec2 vTextPos(m_DrawPos + ImVec2(m_FrameWidth - 10.f - vTextSize.x, floorf((m_FooterHeight * 0.5f) - (vTextSize.y * IMMENU_TEXT_CENTER_VERTICAL))));
					m_FooterText.Draw(m_DrawList, Font.Primary, Font.Primary->FontSize, vTextPos);
				}

				m_DrawPos.y += m_FooterHeight;
			}

			if (Item.IsSelectedValid())
			{
				// Item Description
				auto pItem = Item.GetSelectableItem(Item.m_Selected);
				if (pItem && !pItem->m_Description.empty())
				{
					C_ImMMenuTextMultiColor m_ItemDescription = pItem->GetDescription();

					m_DrawPos.y += 5.f;

					//ImVec2 vTextSize = m_ItemDescription.CalcTextSize(Font.Primary);
					ImVec2 vTextSize = Font.Primary->CalcTextSizeA(Font.Primary->FontSize, m_FrameWidth - 15.f, m_FrameWidth - 15.f, m_ItemDescription.m_Strings[0].c_str());
					float fDescriptionHeight = floorf(vTextSize.y * 0.85f) + 10.f;
					

					//float width = m_FrameWidth + (vTextSize.x > m_FrameWidth ? pItem->m_Description.length() : 0.f);

					m_DrawList->AddRectFilled
					(
						m_DrawPos, 
						m_DrawPos + ImVec2(m_FrameWidth, fDescriptionHeight),
						Color.Description, 
						Item.m_FooterRounded ? Item.m_FooterRounding : 0,
						Item.m_FooterRounded ? ImDrawFlags_RoundCornersBottom : 0
					);

					m_DrawList->AddLine(
						m_DrawPos,
						m_DrawPos + ImVec2(m_FrameWidth, 0.f),
						Color.Footer_Text, 
						3.f);

					ImVec2 vTextPos(m_DrawPos + ImVec2(10.f, 8.f));
					//m_ItemDescription.Draw(m_DrawList, Font.Primary, floorf(Font.Primary->FontSize * 0.85f), vTextPos);
					m_DrawList->AddText(Font.Primary, floorf(Font.Primary->FontSize * 0.95f), vTextPos, IM_COL32_WHITE, m_ItemDescription.m_Strings[0].c_str(), nullptr, m_FrameWidth - 15.f);

					m_DrawPos.y += fDescriptionHeight;
				}

				// InputText Popup
				if (Input.IsInteractingWithInputText)
				{
					C_ImMMenuItemInputText* pInputText = reinterpret_cast<C_ImMMenuItemInputText*>(Item.GetSelectableItem(Item.m_Selected));

					m_DrawPos.y += 5.f;
					ImVec2 m_PopupSize(m_FrameWidth, floorf(Font.Primary->FontSize * 3.f) + 30.f);

					m_DrawList->AddRectFilled(m_DrawPos, m_DrawPos + m_PopupSize, Color.Item);

					pInputText->GetPopupText().Draw(m_DrawList, Font.Primary, Font.Primary->FontSize, m_DrawPos + ImVec2(10.f, 10.f));

					ImRect m_InputFieldBB(m_DrawPos + ImVec2(10.f, Font.Primary->FontSize + 15.f), m_DrawPos + m_PopupSize - ImVec2(10.f, 10.f));
					m_DrawList->AddRectFilled(m_InputFieldBB.Min, m_InputFieldBB.Max, IM_COL32(0, 0, 0, 255));
					m_DrawList->AddRect(m_InputFieldBB.Min, m_InputFieldBB.Max, IM_COL32(50, 50, 50, 255), 0.f, 0, 2.f);

					ImGui::SetNextWindowPos(m_InputFieldBB.Min + ImVec2(5.f, 6.f));
					ImGui::SetNextWindowSize(m_InputFieldBB.Max - m_InputFieldBB.Min);

					ImGui::PushFont(Font.Primary);
					
					ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, Color.Primary);
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4());
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
					if (ImGui::Begin("###InputPopup", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground))
					{
						ImGui::SetCursorPos(ImVec2(0.f, 0.f));
						ImGui::SetKeyboardFocusHere();
						ImGui::SetNextItemWidth(m_InputFieldBB.Max.x - m_InputFieldBB.Min.x - 10.f);
						ImGui::InputText("###InputText", &Item.m_TextInputBuffer[0], pInputText->m_BufferSize, pInputText->m_Flags);
						ImGui::End();
					}
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(2);
					ImGui::PopFont();
			
				}
			}

			// Input
			if (Input.IsInteractingWithKeybind)
			{
				ImGuiKey iPressedKey = Input.GetAnyPressed();
				if (iPressedKey != ImGuiKey_None)
				{
					C_ImMMenuItemKeybind* pInteractedItem = reinterpret_cast<C_ImMMenuItemKeybind*>(Item.GetInteracted());
					*pInteractedItem->m_Value = (iPressedKey == ImGuiKey_Escape ? ImGuiKey_None : iPressedKey);

					Input.IsInteractingWithKeybind = false;
				}
			}
			else if (Input.IsInteractingWithInputText)
			{
				if (ImGui::IsKeyPressed(Input.m_InteractionKey, false))
				{
					C_ImMMenuItemInputText* pInputText = reinterpret_cast<C_ImMMenuItemInputText*>(Item.GetSelectableItem(Item.m_Selected));
					memcpy(pInputText->m_Buffer, &Item.m_TextInputBuffer[0], pInputText->m_BufferSize);
					Item.m_TextInputBuffer.clear();
					Input.IsInteractingWithInputText = false;
				}
				else if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
					Input.IsInteractingWithInputText = false;
				}
			}
			else if (!Input.BlockedByItem())
			{
				// BUG: It does scroll properly, but using this with arrows will make next Item.Update get stuck inside the infinite while loop...
				/*if (Input.m_CaptureMouse)
				{
					float m_Scroll = ImGui::GetIO().MouseWheel;
					if (m_Scroll != 0.f)
					{
						int m_ItemCount = Item.GetCount();
						int m_ItemSelectableCount = Item.GetSelectableCount();
						int m_MaxIndex = ImMax(0, (m_ItemCount - Item.m_NumToShow));

						int m_ScrollCount = (m_Scroll > 0.f ? -1 : 1);
						Item.m_Index = ImClamp(Item.m_Index + m_ScrollCount, 0, m_MaxIndex);
					}
				}*/

				if (Input.m_SelectUpDown != 0)
				{
					Item.Update(static_cast<int>(Input.m_SelectUpDown));
					ItemNameScroll.Reset(m_CurrentTime, (IMMENU_ITEM_NAME_SCROLL_WAIT_TIME * 0.5));
					Input.m_SelectUpDown = 0;
				}

				if (Input.m_SelectLeftRight != 0)
				{
					if (Item.IsSelectedValid()) {
						Item.UpdateSideInteraction(static_cast<int>(Input.m_SelectLeftRight));
					}

					Input.m_SelectLeftRight = 0;
				}

				if (Input.m_SelectInteraction)
				{
					if (Item.IsSelectedValid())
					{
						Item.m_Interacted = Item.m_Selected;
						Item.UpdateInteraction();

						C_ImMMenuItem* pInteractedItem = Item.GetInteracted();
						if (pInteractedItem)
						{
							if (pInteractedItem->m_Type == eImMMenuItemType_Keybind) {
								Input.IsInteractingWithKeybind = true;
							}
							else if (pInteractedItem->m_Type == eImMMenuItemType_InputText)
							{
								Item.m_Interacted = -1;
								Item.m_TextInputBuffer = reinterpret_cast<C_ImMMenuItemInputText*>(pInteractedItem)->m_Buffer;
								Item.m_TextInputBuffer.resize(reinterpret_cast<C_ImMMenuItemInputText*>(pInteractedItem)->m_BufferSize);
								Input.IsInteractingWithInputText = true;
							}
						}
					}

					Input.m_SelectInteraction = false;
				}
				else {
					Item.m_Interacted = -1;
				}
				if (ImGui::IsKeyPressed(ImGuiKey_Keypad0, false)) {
					Submenus::Back();
				}
			}

			Item.CleanUp();
		}
	};

}
