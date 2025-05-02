#pragma once

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
		virtual void RenderOption() = 0;
	public:
		const char* GetText()
		{
			return m_OptionText;
		}
		const char* GetDescription()
		{
			return m_OptionDescription;
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
	private:
		const char* m_OptionText = "BaseOption";
		const char* m_OptionDescription = "BaseOption Description";
		OptionType m_OptionType;
	};
}