#include "hooking.h"
#include "methods/detour.h"
#include "../../kiero/kiero.h"


namespace cheat
{
	hooking::hooking()
	{
		auto status = MH_Initialize();
		if (status != MH_OK)
		{
			g_logger->send(levels::critical, "MinHook Initialize Failed: {}", MH_StatusToString(status));
		}

		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			g_logger->send(levels::debug, "DX11");
		}

		this->Hook();
	}
	hooking::~hooking()
	{
		this->UnHook();
	}

	void hooking::Hook()
	{
		// D3D11
		kiero::bind(8, (void**)&hooks::ogPresent, hooks::Present);
	}

	void hooking::UnHook()
	{
		g_Detour.unhook();

	}
}