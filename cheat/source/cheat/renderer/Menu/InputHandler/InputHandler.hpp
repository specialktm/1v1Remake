#pragma once
#include "../../pch/framework.h"
class InputHandler
{
public:
	InputHandler(const std::pair<ImGuiKey, ImGuiKey>& pKeys, bool pRequireBoth, std::chrono::milliseconds pPressDelay = 300ms);
public:
	bool IsPressed();
private:
	std::pair<ImGuiKey, ImGuiKey> m_Keys;
	std::chrono::milliseconds m_PressDelay;
	std::chrono::steady_clock::time_point m_LastPressed;
	bool m_RequireBoth = false;
};

inline InputHandler MainMenuKeyboard({ ImGuiKey_F4, ImGuiKey_Insert }, false);
inline InputHandler MainMenuController({ ImGuiKey_GamepadDpadDown ,ImGuiKey_GamepadL1 }, true);

inline InputHandler UpKey({ ImGuiKey_GamepadDpadUp ,ImGuiKey_Keypad8 }, false, 10ms);
inline InputHandler DownKey({ ImGuiKey_GamepadDpadDown ,ImGuiKey_Keypad2 }, false, 10ms);
inline InputHandler LeftKey({ ImGuiKey_GamepadDpadLeft ,ImGuiKey_Keypad4 }, false, 10ms);
inline InputHandler RightKey({ ImGuiKey_GamepadDpadRight ,ImGuiKey_Keypad6 }, false, 10ms);
inline InputHandler EnterKey({ ImGuiKey_GamepadFaceDown ,ImGuiKey_Keypad5 }, false, 10ms);
inline InputHandler BackKey({ ImGuiKey_GamepadFaceRight ,ImGuiKey_Keypad0 }, false, 10ms);