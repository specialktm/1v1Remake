#include "Interface.hpp"

namespace cheat
{
	void Interface::RenderInterface()
	{
		if (!m_ShouldRenderInterface)
			return;

		switch (m_InterfaceType)	
		{
			case InterfaceType::ListInterface:
				RenderListInterface();
				break;
			case InterfaceType::ClickInterface:
				RenderClickInterface();
				break;
			default:
				RenderListInterface();
		}
	}

	void Interface::RenderListInterface()
	{

	}

	void Interface::RenderClickInterface()
	{
		return;
	}
}