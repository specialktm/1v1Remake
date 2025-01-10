#pragma once
#include "../../renderer.hpp"
#include "Types/Player.h"

namespace cheat::Submenus
{
	void (*FunctionPointers[])();
	extern int NumberOfSubmenus;

	void Home();
	// Menu
	void Tick();
	void Move(void* Submenu);
	void Back();
	void NewSub(const char* name, void* Submenu);
	void Title(const char* title);  
	bool Button(const char* name,std::function<void()> callback, const char* desc = "");
	void Break(const char* name = "");
	bool Bool(const char* name, const char* m_Description, bool* p_Value);
	bool Float(const char* name, const char* m_Description, float* p_Value, float m_Min, float m_Max, float m_Power = 0.1f, bool m_Clamp = false);
	


	namespace Menus
	{
		// Texture
		void TextureHeader();
		void TextureSubtitle();
		void TextureBackground();
		void TextureScroller();
		void TextureFooter();
		void TextureFooter();
		void MenuESP();
		// Theme
		void TextureThemes();
		//Settings
		void MenuSettings();
		// Selected Player
		void SelectedPlayer();
		void Players();
	}

}