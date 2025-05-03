#include "InputHandler.hpp"

InputHandler::InputHandler(const std::pair<ImGuiKey, ImGuiKey>& pKeys, bool pRequireBoth, std::chrono::milliseconds pPressDelay)
	:
	m_Keys(pKeys), m_RequireBoth(pRequireBoth), m_PressDelay(pPressDelay) 
   {}

bool InputHandler::IsPressed()
{
	auto now = std::chrono::steady_clock::now();
    if (now - m_LastPressed > m_PressDelay)
    {
        bool pressed = false;
        if (!m_RequireBoth)
        {
            pressed = ImGui::IsKeyPressed(m_Keys.first, true) || ImGui::IsKeyPressed(m_Keys.second, true);
        }
        else
        {
            pressed = ImGui::IsKeyPressed(m_Keys.first, true) && ImGui::IsKeyPressed(m_Keys.second, true);
        }

        if (pressed)
        {
            m_LastPressed = now;
            return true;
        }
    }

	return false;
}