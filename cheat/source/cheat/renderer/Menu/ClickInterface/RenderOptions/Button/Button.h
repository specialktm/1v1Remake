#pragma once
#include <functional>

#include "../../imgui/imgui.h"

namespace CheatRender {

    class Button
    {
    public:
        using Callback = std::function<void()>;

        Button(const char* label,Callback onClick = nullptr, ImVec2 Size = {0.f,0.f});

        void Render();

    private:
        const char* m_Label;
        Callback m_OnClickCallback;
        ImVec2 m_Size;
    };

}
