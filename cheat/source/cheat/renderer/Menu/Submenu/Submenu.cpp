#include "Submenu.h"

namespace cheat::Submenus
{
	void (*FunctionPointers[])() = {
		Home,
        Menus::TextureHeader,
        Menus::TextureSubtitle,
        Menus::TextureBackground,
        Menus::TextureScroller,
        Menus::TextureFooter,
        Menus::TextureThemes,
        Menus::MenuSettings,
        Menus::MenuESP
	};
	int NumberOfSubmenus = sizeof(Submenus::FunctionPointers) / sizeof(Submenus::FunctionPointers[0]);

	void Tick()
	{
		for (int FuncPointerIndex = 0; FuncPointerIndex < Submenus::NumberOfSubmenus; ++FuncPointerIndex)
		{
			if (g_Renderer->currentMenu == (*Submenus::FunctionPointers[FuncPointerIndex]))
			{
				(*Submenus::FunctionPointers[FuncPointerIndex])();
			}
		}
	}

        
    void Move(void* Submenu)
    {
        g_Renderer->menusArray.push_back(g_Renderer->currentMenu);
        g_Renderer->optionsArray.push_back(g_Renderer->Menu.Item.m_Selected);
        g_Renderer->menuLevel++;
        g_Renderer->currentMenu = Submenu;
        g_Renderer->Menu.Item.m_Selected = 0;
    }

    void Back()
    {
        if (g_Renderer->currentMenu == Submenus::Home)
        {
            g_Renderer->m_Open = false;
        }
        else
        {

            g_Renderer->Menu.Item.ResetSelection();
            g_Renderer->currentMenu = g_Renderer->menusArray.back();
            g_Renderer->Menu.Item.m_Selected = g_Renderer->optionsArray.back();
            g_Renderer->menusArray.pop_back();
            g_Renderer->optionsArray.pop_back();
            g_Renderer->menuLevel--;

        }
    }

    void NewSub(const char* name, void* submenu)
    {
        if (g_Renderer->Menu.Item.AddSection(name))
        {
            Move(submenu);
        }
    }

    void Title(const char* title)
    {
        g_Renderer->Menu.SetTitleText(title);
    }

    bool Button(const char* name)
    {
        return g_Renderer->Menu.Item.AddText(name);
    }

    void Break(const char* name)
    {
        g_Renderer->Menu.Item.AddSeparator(name);
    }

    bool Bool(const char* name, const char* m_Description, bool* p_Value)
    {
        return g_Renderer->Menu.Item.AddCheckbox(name,m_Description, p_Value);
    }

    bool Float(const char* name, const char* m_Description, float* p_Value, float m_Min, float m_Max, float m_Power, bool m_Clamp)
    {
        return g_Renderer->Menu.Item.AddFloat(name, m_Description, p_Value, m_Min, m_Max,m_Power, "%.3f", m_Clamp);
    }
}