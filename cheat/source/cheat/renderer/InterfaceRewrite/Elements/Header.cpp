#include "../Interface.hpp"

namespace cheat
{
	void Interface::ListInterface::RenderHeader()
	{
		ImVec2 HeaderPosition{ MenuWidth + DrawPosition.x, Header.Height + DrawPosition.y };
		if (Header.Image == nullptr)
		{
			BackgroundDrawlist->AddRectFilled(
				DrawPosition,
				HeaderPosition,
				Header.Color,
				Header.IsRounded ? Header.Rounding : 0,
				Header.IsRounded ? ImDrawFlags_RoundCornersTop : 0
			);
		}
		if (Header.ShouldDrawText)
			ForegroundDrawlist->AddText(HeaderPosition, IM_COL32_WHITE, Header.Text);
		DrawPosition.y += Header.Height;
	}
}