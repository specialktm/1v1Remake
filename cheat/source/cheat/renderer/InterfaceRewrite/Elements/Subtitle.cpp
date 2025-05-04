#include "../Interface.hpp"
#include "../../../../imgui/imgui_internal.h"

namespace cheat
{
	void Interface::ListInterface::RenderSubtitle()
	{
		const float SubtitleHeight = ImFloor(ImGui::GetDefaultFont()->FontSize * 2.f);
		ImVec2 SubtitleBottomRight{ DrawPosition.x + MenuWidth, DrawPosition.y + SubtitleHeight };

		BackgroundDrawlist->AddRectFilled(
			DrawPosition,
			SubtitleBottomRight,
			Subtitle.Color
		);

		const char* leftText = Subtitle.Text ? Subtitle.Text : "";
		ImVec2 leftTextPos{
			DrawPosition.x + 5.0f,
			DrawPosition.y + (SubtitleHeight - ImGui::GetFontSize()) * 0.5f
		};
		ForegroundDrawlist->AddText(
			leftTextPos,
			ImColor(255, 255, 255, 255),
			leftText
		);

		char rightBuffer[64];
		snprintf(rightBuffer, sizeof(rightBuffer), "Selected: %d / %d", m_Selected + 1, NumberOfOptions);
		ImVec2 rightTextSize = ImGui::CalcTextSize(rightBuffer);
		ImVec2 rightTextPos{
			DrawPosition.x + MenuWidth - rightTextSize.x - 5.0f,
			DrawPosition.y + (SubtitleHeight - rightTextSize.y) * 0.5f
		};
		ForegroundDrawlist->AddText(
			rightTextPos,
			ImColor(255, 255, 255, 255),
			rightBuffer
		);

		DrawPosition.y += SubtitleHeight;
	}

}