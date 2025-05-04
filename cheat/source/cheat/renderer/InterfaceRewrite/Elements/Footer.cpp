#include "../Interface.hpp"

namespace cheat
{
	void Interface::ListInterface::RenderFooter()
	{
		const float FooterHeight = OptionHeight;
		ImVec2 FooterBottomRight = DrawPosition + ImVec2{ MenuWidth, FooterHeight };

		BackgroundDrawlist->AddRectFilled(
			DrawPosition,
			FooterBottomRight,
			Footer.Color
		);

		const char* leftText = Footer.Text ? Footer.Text : "";
		ImVec2 leftTextPos{
			DrawPosition.x + 5.0f,
			DrawPosition.y + (FooterHeight - ImGui::GetFontSize()) * 0.5f
		};
		BackgroundDrawlist->AddText(
			leftTextPos,
			ImColor(255, 255, 255, 255),
			leftText
		);

		const char* rightText = Footer.Version ? Footer.Version : "";
		ImVec2 rightTextSize = ImGui::CalcTextSize(rightText);
		ImVec2 rightTextPos{
			DrawPosition.x + MenuWidth - rightTextSize.x - 5.0f,
			DrawPosition.y + (FooterHeight - rightTextSize.y) * 0.5f
		};

		BackgroundDrawlist->AddText(
			rightTextPos,
			ImColor(255, 255, 255, 255),
			rightText
		);
	}

}
