#include "../Interface.hpp"

namespace cheat
{
	void Interface::ListInterface::RenderBackground()
	{
		float BackgroundHeight = 0.f;

		if (NumberOfOptions > MaxNumberOfOptionsVisible)
			BackgroundHeight = MaxNumberOfOptionsVisible * OptionHeight;
		else
			BackgroundHeight = NumberOfOptions * OptionHeight;

		ImVec2 BackgroundBottomRight{
			DrawPosition.x + MenuWidth,
			DrawPosition.y + BackgroundHeight
		};

		BackgroundDrawlist->AddRectFilled(
			DrawPosition,
			BackgroundBottomRight,
			Background.Color
		);
	}

}
