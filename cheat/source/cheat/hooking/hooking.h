#pragma once
#include "../../pch/pch.h"
#include "../../util/Il2cpp_Resolver/il2cpp_resolver.hpp"
#include <il2cpp.h>

struct Rect // Anvar :)
{
	float left, right{};
	float top, bottom{};
	ImVec2 size{};

	void get(const RECT& handle)
	{
		left = static_cast<float>(handle.left);
		right = static_cast<float>(handle.right);
		top = static_cast<float>(handle.top);
		bottom = static_cast<float>(handle.bottom);
		size.x = right - left;
		size.y = bottom - top;
	}
};

namespace cheat::D3D11
{
	inline HWND m_window = NULL;
	inline std::string_view m_WindowName;
	inline Rect m_WindowRect;
	inline comPtr<ID3D11Device> m_Device;
	inline comPtr<ID3D11DeviceContext> m_Context;
	inline comPtr<ID3D11RenderTargetView> m_mainRenderTargetView;
}

namespace cheat::types
{
	//D3D11
	typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
}

namespace cheat::offsets
{
	inline uintptr_t GameAssembly;
	inline uintptr_t UnityPlayer;
	inline uintptr_t AddRecoil;
	inline uintptr_t DiscordUpdate;
}

namespace cheat::hooks
{
	// D3D11
	HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	inline types::Present ogPresent;

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	inline types::WNDPROC ogWndProc;

	void AddRecoil(vThirdPersonCamera_o* _this, UnityEngine_Vector2_o recoil, float recoilDuration, float returnForce, const MethodInfo* method);
	inline void(UNITY_CALLING_CONVENTION ogAddRecoil)(vThirdPersonCamera_o* _this, UnityEngine_Vector2_o recoil, float recoilDuration, float returnForce, const MethodInfo* method);
}


namespace cheat
{
	inline std::vector<Unity::CGameObject*> PlayerList(NULL);
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