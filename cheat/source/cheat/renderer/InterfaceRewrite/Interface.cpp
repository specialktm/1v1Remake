#include "Interface.hpp"
#include "../../../imgui/imgui_internal.h"
#include "../Menu/InputHandler/InputHandler.hpp"

namespace cheat
{
	void Interface::RenderInterface()
	{
		if (!m_ShouldRenderInterface)
			return;

		switch (m_InterfaceType)
		{
			case InterfaceType::ListInterface:
				ListInterface.Render();
				break;
			case InterfaceType::ClickInterface:
				ClickInterface.Render();
				break;
			default:
				ListInterface.Render();
		}
	}
    void Interface::ListInterface::HandleTheInput()
    {
        int pSelectionChange = 0;
        if (UpKey.IsPressed())
            pSelectionChange -= 1;
        if (DownKey.IsPressed())
            pSelectionChange += 1;


        int m_ItemCount = GetOptionCount();
        int m_ItemSelectableCount = SelectableCount();

        int m_MaxIndex = (m_ItemCount - MaxNumberOfOptionsVisible);

        if (m_ItemSelectableCount > 0)
        {
            m_Selected += pSelectionChange;
            if (0 > m_Selected)
            {
                m_Selected = (m_ItemSelectableCount - 1);
                m_Index = ImClamp(GetSelectable(m_Selected) - MaxNumberOfOptionsVisible + 1, 0, ImMax(m_MaxIndex, 0));
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
                    if (m_SelectedInList >= m_Index && (m_Index + MaxNumberOfOptionsVisible) > m_SelectedInList)
                        break;

                    m_Index = (m_Index + pSelectionChange) % (m_MaxIndex + 1);
                }
            }
            if ((EnterKey.IsPressed() || ImGui::IsMouseClicked(0)) && m_Selected >= 0) {
                int selectedIndex = GetSelectable(m_Selected);
                if (selectedIndex >= 0 && selectedIndex < OptionsVector.size()) {
                    OptionsVector[selectedIndex].HandleInput(BaseOption::OptionInput::Enter);
                }
            }
        }
        else
        {
            m_Index = 0;
            m_Selected = -1;
        }

    }

    void Interface::ListInterface::Render()
    {
        // Setup drawing context
        BackgroundDrawlist = ImGui::GetBackgroundDrawList();
        ForegroundDrawlist = ImGui::GetForegroundDrawList();
        DrawPosition = {
            ImMin(IO->DisplaySize.x - MenuWidth, floorf(Position.x * IO->DisplaySize.x)),
            floorf(Position.y * IO->DisplaySize.y)
        };

        // Store original position
        ImVec2 originalDrawPosition = DrawPosition;

        // Process input
        HandleTheInput();

        // Calculate smooth scroll position
        static float smoothScrollY = DrawPosition.y;
        smoothScrollY = lerp(smoothScrollY, DrawPosition.y, Scroller.Smoothing);

        RenderHeader();
        RenderSubtitle();
        RenderBackground();

        float currentY = smoothScrollY; 
        ImVec2 mousePos = ImGui::GetMousePos();
        float deltaTime = ImGui::GetIO().DeltaTime;

        for (int i = m_Index; i < ImMin(static_cast<int>(OptionsVector.size()), m_Index + NumberOfOptions); ++i)
        {
            BaseOption& option = OptionsVector[i];

            const ImVec2 textSize = ImGui::CalcTextSize(option.GetText());
            const float optionHeight = floorf(textSize.y * 2.0f);

            float targetY = currentY;
            static std::unordered_map<int, float> optionYPositions;
            if (optionYPositions.find(i) == optionYPositions.end()) {
                optionYPositions[i] = targetY;
            }
            optionYPositions[i] = lerp(optionYPositions[i], targetY, Scroller.Smoothing * 2.0f);

            const ImVec2 optionTopLeft = {
                DrawPosition.x,
                optionYPositions[i]
            };
            const ImVec2 optionBottomRight = {
                DrawPosition.x + MenuWidth,
                optionYPositions[i] + optionHeight
            };

            bool isSelected = (option.IsSelecable() && m_Selected >= 0 && i == GetSelectable(m_Selected));

            static float highlightAlpha = 0.0f;
            highlightAlpha = lerp(highlightAlpha, isSelected ? 1.0f : 0.0f, Scroller.Smoothing * 4.0f);

            if (highlightAlpha > 0.01f) {
                ImColor highlightColor = Scroller.Color;
                highlightColor.Value.w = highlightAlpha;
                BackgroundDrawlist->AddRectFilled(
                    optionTopLeft,
                    optionBottomRight,
                    highlightColor
                );
            }

            static ImColor textColorTarget = ImColor(255, 255, 255);
            textColorTarget = isSelected ? ImColor(0, 0, 0) : ImColor(255, 255, 255);
            ImColor currentTextColor = ImColor(
                lerp(textColorTarget.Value.x, textColorTarget.Value.x, Scroller.Smoothing * 4.0f),
                lerp(textColorTarget.Value.y, textColorTarget.Value.y, Scroller.Smoothing * 4.0f),
                lerp(textColorTarget.Value.z, textColorTarget.Value.z, Scroller.Smoothing * 4.0f)
            );

            ImVec2 textPos = {
                DrawPosition.x + 10.0f,
                optionYPositions[i] + (optionHeight - textSize.y) * 0.5f
            };

            ForegroundDrawlist->AddText(textPos, currentTextColor, option.GetText());

            if (isSelected && option.GetDescription() && strlen(option.GetDescription()) > 0) {
                ImVec2 descPos = { textPos.x, optionYPositions[i] + optionHeight + 2.0f };
                ForegroundDrawlist->AddText(
                    descPos,
                    ImColor(180, 180, 180, static_cast<int>(highlightAlpha * 255)),
                    option.GetDescription()
                );
            }

            currentY += optionHeight;
        }

        static float footerY = originalDrawPosition.y;
        footerY = lerp(footerY, currentY, Scroller.Smoothing);

        DrawPosition = originalDrawPosition;
        DrawPosition.y = footerY;
        RenderFooter();
    }

	void Interface::ClickInterface::Render()
	{
		return;
	}
}