#pragma once
#include "../../imgui/imgui.h"
namespace cheat
{
	class BaseOption
	{
	public:
		enum class OptionType : unsigned char
		{
			Break = 0U,
			Button,
			Toggle,
			Submenu,
			Float,
			Int,
			Vector,
			IconButton
		};
		enum class OptionInput 
		{
			Up,
			Down,
			Left,
			Right,
			Enter,
			Back
		};
		enum class ToggleType : unsigned char
		{
			RegularToggle = 0U,
			VectorToggle,
			IntToggle,
			FloatToggle
		};
		enum AllineText : unsigned char
		{
			Left = 0U,
			Right,
			Center
		};
	public:
		BaseOption() = default;
		~BaseOption() = default;
	public:
		virtual void RenderOption(ImVec2 pOptionPosition) {};
	public:
		const char* GetText()
		{
			return m_OptionText;
		}
		const char* GetDescription()
		{
			return m_OptionDescription;
		}
		void SetType(OptionType pType)
		{
			m_OptionType = pType;
		}
		void SetText(const char* pOptionText)
		{
			m_OptionText = pOptionText;
		}
		void SetDescription(const char* pOptionDescription)
		{
			m_OptionDescription = pOptionDescription;
		}
		OptionType GetType()
		{
			return m_OptionType;
		}
		const char* GetOptionId()
		{
			return m_OptionId;
		}	
		void SetOptionId(const char* id)
		{
			m_OptionId = id;
		}
		bool IsSelecable()
		{
			bool Selectable = false;
			GetType() == OptionType::Break ? Selectable = false : Selectable = true;
			return Selectable;
		}

		virtual bool HandleInput(OptionInput input) {};

		virtual bool OnConfirm() { return false; }
		virtual bool OnBack() { return false; }
		virtual bool CanOptionReceiveInput() { return IsSelecable(); }

	private:
		const char* m_OptionText = "BaseOption";
		const char* m_OptionDescription = "BaseOption Description";
		OptionType m_OptionType;
		const char* m_OptionId = "";

	};
}