#include "../../renderer.hpp"
#include "RenderOptions/Checkbox/CheckBox.h"
#include "RenderOptions/Button/Button.h"

namespace cheat
{
	bool m_TestFeature;
	void renderer::RenderClickInterface()
	{
        if (g_Renderer->m_Open && !g_Renderer->m_ListInterface)
        {
            ImGui::GetIO().MouseDrawCursor = true;
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            ImGui::GetIO().MouseDrawCursor = false;
            ImGui::GetIO().ConfigFlags |= ~ImGuiConfigFlags_NoMouse;
        }

		ImGui::Text("%s", APP_NAME);

		CheatRender::CheckBox("Tesing Toggle", "Tesing with duh gang :sunglasses:", &m_TestFeature).Render();
		CheatRender::Button("Switch Interface", [this] { this->SetInterface(true); }).Render();
	}
}