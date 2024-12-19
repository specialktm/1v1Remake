#include "../hooking.h"
#include "../../renderer/renderer.hpp"
#include "../../../common/common.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace cheat
{

	LRESULT __stdcall hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		using namespace D3D11;
		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProcA(ogWndProc, hWnd, uMsg, wParam, lParam);
	}
	static bool PresentHooked = false;
	HRESULT __stdcall hooks::Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		using namespace D3D11;
		if (!PresentHooked)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&m_Device)))
			{
				m_Device->GetImmediateContext(&m_Context);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				m_window = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_mainRenderTargetView);
				pBackBuffer->Release();
				hooks::ogWndProc = (types::WNDPROC)SetWindowLongPtr(m_window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
				ImGui_ImplWin32_Init(m_window);
				ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get());
				g_Renderer->Style();
				g_Renderer->Menu.Initialize();
				PresentHooked = true;
		
			}
			else
			{
				return ogPresent(pSwapChain, SyncInterval, Flags);
			}
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		g_Renderer->OnPresent();
		ImGui::Render();
		m_Context->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return ogPresent(pSwapChain, SyncInterval, Flags);
	}
}