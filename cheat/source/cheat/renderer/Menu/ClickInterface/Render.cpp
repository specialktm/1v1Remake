#include "../../renderer.hpp"
#include "RenderOptions/Checkbox/CheckBox.h"
#include "RenderOptions/Button/Button.h"

namespace cheat
{
	bool m_TestFeature;
	void renderer::RenderClickInterface()
	{
		ImGui::Text("%s", APP_NAME);

		CheatRender::CheckBox("Tesing Toggle", "Tesing with duh gange :sunglasses:", &m_TestFeature).Render();
		CheatRender::Button("Switch Interface", [this] { this->SetInterface(true); }).Render();
	}
}