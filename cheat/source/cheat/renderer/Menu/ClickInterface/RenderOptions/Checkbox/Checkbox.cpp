#include  "CheckBox.h"
#include "Element/Toggle.h"
namespace CheatRender
{
    CheckBox::CheckBox(const char* label,const char* Description, bool* value, Callback onChecked, Callback onUnChecked)
        : m_Label(label), m_Description(Description), m_Value(value), m_OnCheckedCallback(onChecked), m_OnUnCheckedCallback(onUnChecked)
    {
    }

    void CheckBox::Render()
    {
        if (Elements::Toggle(m_Label, m_Description, m_Value))
        {
            if (*m_Value)
            {
                CallOnChecked();
            }
            else
            {
                CallOnUnChecked();
            }
        }
    }

    void CheckBox::SetOnCheckedCallback(Callback callback)
    {
        m_OnCheckedCallback = callback;
    }

    void CheckBox::SetOnUnCheckedCallback(Callback callback)
    {
        m_OnUnCheckedCallback = callback;
    }

    void CheckBox::SetText(const char* label ...)
    {
        m_Label = label;
    }
    const char* CheckBox::GetText()
    {
        return m_Label;
    }
    void CheckBox::SetDescription(const char* description)
    {
        m_Description = description;
    }
    const char* CheckBox::GetDescription()
    {
        return m_Description;
    }

}
