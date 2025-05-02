#pragma once
#include "../option.hpp"

namespace cheat
{
	class IconOption : public BaseOption
	{

	public:
		const char* GetIcon()
		{
			return m_Icon;
		}
		AllineText GetIconAllinement()
		{
			return m_IconAllinement;
		}
		void SetIcon(const char* pIcon)
		{
			m_Icon = pIcon;
		}
		void SetIconAllinement(AllineText pAllinement)
		{
			m_IconAllinement = pAllinement;
		}
	private:
		const char* m_Icon = "Unknown";
		AllineText m_IconAllinement = Right;
	};
}