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
        Menus::TextureDescription,
        Menus::TextureThemes,
        Menus::MenuSettings,
        Menus::MenuESP,
        Menus::Players,
        Menus::SelectedPlayer,
    //    Menus::Icons
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
        int m_SelectableCount = g_Renderer->Menu.Item.GetSelectableCount();
        if (g_Renderer->Menu.Item.m_Selected >= m_SelectableCount)
        {
            g_Renderer->Menu.Item.m_Index = 0;
            g_Renderer->Menu.Item.SetSelected((m_SelectableCount ? 0 : -1));
        }
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
            int m_SelectableCount = g_Renderer->Menu.Item.GetSelectableCount();
            if (g_Renderer->Menu.Item.m_Selected >= m_SelectableCount)
            {
                g_Renderer->Menu.Item.m_Index = g_Renderer->Menu.Item.m_Selected;
            }
        }
    }

    void Submenu(const char* name, void* submenu, const char* icon)
    {
        if (g_Renderer->Menu.Item.AddSection(name, icon))
        {
            Move(submenu);
        }
    } 

    void Title(const char* title)
    {
        g_Renderer->Menu.SetTitleText(title);
    }

    void Button(const char* name, std::function<void()> callback, const char* desc)
    {
        if (g_Renderer->Menu.Item.AddText(name,desc))
        {
            queue::add([callback] {
                 callback();
            });
        }
    }
    void IconButton(const char* icon, const char* name, std::function<void()> callback, const char* desc)
    {
        if (g_Renderer->Menu.Item.AddIcon(icon,name, desc))
        {
            queue::add([callback] {
                callback();
            });
        }
       
    }
    void Break(const char* name)
    {
        g_Renderer->Menu.Item.AddSeparator(name);
    }
    void Dummy(const char* name)
    {
        g_Renderer->Menu.Item.AddTextUnselectable(name);
    }

    bool Bool(const char* name, const char* m_Description, bool* p_Value)
    {
        return g_Renderer->Menu.Item.AddCheckbox(name,m_Description, p_Value);
    }

    bool Float(const char* name, const char* m_Description, float* p_Value, float m_Min, float m_Max, float m_Power, bool m_Clamp)
    {
        return g_Renderer->Menu.Item.AddFloat(name, m_Description, p_Value, m_Min, m_Max,m_Power, "%.2f", m_Clamp);
    }

    bool Number(const char* m_Name, const char* m_Description, int* p_Value, int m_Min, int m_Max, int m_Power, bool m_Clamp)
    {
        return g_Renderer->Menu.Item.AddInteger(m_Name, m_Description, p_Value, m_Min, m_Max, m_Power, m_Clamp);
    }
    bool Input(const char* m_Name, const char* m_Description, std::string m_PopupText, char* m_Buffer, size_t m_BufferSize, ImGuiInputTextFlags m_Flags, bool m_HideBuffer)
    {
        return g_Renderer->Menu.Item.AddTextInput(m_Name, m_Description, m_PopupText, m_Buffer, m_BufferSize, m_Flags, m_HideBuffer);
    }
}