#pragma once
#include "../BaseOption.hpp"
#include <functional>
#include <string>
#include <format>        
#include <algorithm>      
#include "../../../../../imgui/imgui.h"

namespace cheat
{
    class ButtonOption : public BaseOption {
    public:
        ButtonOption(const char* buttonText, const char* buttonDescription = "", std::function<void()> callback = nullptr)
        {
            SetText(buttonText);
            SetType(OptionType::Button);
            SetDescription(buttonDescription);

            std::string id = std::format("{}+{}", buttonText, std::rand() % 1000);
            SetOptionId(id.c_str());

            m_Callback = std::move(callback);
        }

        void RenderOption(ImVec2 pOptionPosition) override
        {
            ImColor textColor = ImColor(255, 255, 255, 255);

            ImVec2 textSize = ImGui::CalcTextSize(GetText());
            ImVec2 textPos = {
                pOptionPosition.x + (420.f - textSize.x) * 0.5f, 
                pOptionPosition.y
            };

            ImGui::GetForegroundDrawList()->AddText(textPos, textColor, GetText());

            if (GetDescription() && strlen(GetDescription()) > 0) {
                ImVec2 descPos = {
                    pOptionPosition.x + 10.0f,
                    pOptionPosition.y + textSize.y + 5.0f
                };
                ImGui::GetForegroundDrawList()->AddText(
                    descPos,
                    ImColor(200, 200, 200, 255),
                    GetDescription()
                );
            }
        }

        bool HandleInput(OptionInput input) override
        {
            switch (input) {
                case OptionInput::Enter:
                    Trigger();
                    return true;
                case OptionInput::Left:
                case OptionInput::Right:
                    return false;
                default:
                    return BaseOption::HandleInput(input);
            }
        }

        void Trigger() const
        {
            if (m_Callback)
                m_Callback();
        }

    private:
        std::function<void()> m_Callback;
    };
}
