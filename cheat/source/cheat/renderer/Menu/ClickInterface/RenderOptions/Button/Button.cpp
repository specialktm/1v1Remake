#include "Button.h"


namespace CheatRender
{
    Button::Button(const char* label, Callback onClick, ImVec2 Size)
        : m_Label(label), m_OnClickCallback(onClick), m_Size(Size)
    {
    }

    void Button::Render()
    {
        if (ImGui::Button(m_Label, m_Size))
        {
            if (m_OnClickCallback)
            {
                m_OnClickCallback();
            }
        }
    }
}
