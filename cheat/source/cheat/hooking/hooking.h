#pragma once
#include "../../pch/pch.h"
#include <il2cpp.h>

namespace cheat::D3D11
{
	inline HWND m_window = NULL;
	inline comPtr<ID3D11Device> m_Device;
	inline comPtr<ID3D11DeviceContext> m_Context;
	inline ID3D11RenderTargetView* m_mainRenderTargetView;
}

namespace cheat::types
{
	//D3D11
	typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
}

namespace cheat::hooks
{
	// D3D11
	HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	inline types::Present ogPresent;

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	inline types::WNDPROC ogWndProc;
}



namespace cheat
{

	class hooking
	{

	public:
		hooking();
		~hooking();
	public:

	private:
		void Hook();
		void UnHook();

	};
	inline std::unique_ptr<hooking> g_Hooking{};
}