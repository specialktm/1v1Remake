#pragma once
#include <functional>
#include "../../imgui/imgui.h"
#include <string>  
#include <iostream>  

namespace CheatRender
{

    class CheckBox
    {
    public:
        using Callback = std::function<void()>;

        CheckBox(const char* label, const char* Description, bool* value, Callback onChecked = nullptr, Callback onUnChecked = nullptr);
        void Render();
        void SetOnCheckedCallback(Callback callback);
        void SetOnUnCheckedCallback(Callback callback);
        void SetText(const char* label ...);
        void SetDescription(const char* description);
        const char* GetText();
        const char* GetDescription();
    private:
        void CallOnChecked()
        {
            if (m_OnCheckedCallback)
            {
                m_OnCheckedCallback();
            }
        }

        void CallOnUnChecked()
        {
            if (m_OnUnCheckedCallback)
            {
                m_OnUnCheckedCallback(); 
            }
        }

        const char* m_Label; 
        const char* m_Description = m_Label; 
        bool* m_Value; 
        Callback m_OnCheckedCallback;
        Callback m_OnUnCheckedCallback; 
    };

}
