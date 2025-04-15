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
        if (!g_Running) return ogPresent(pSwapChain, SyncInterval, Flags);

        if (!PresentHooked)
        {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)m_Device.GetAddressOf())))
            {
                m_Device->GetImmediateContext(m_Context.GetAddressOf());

                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                m_window = sd.OutputWindow;

                RECT tmpRect{};
                if (GetWindowRect(sd.OutputWindow, &tmpRect))
                {
                    m_WindowRect.get(tmpRect);
                }

                ID3D11Texture2D* pBackBuffer = nullptr;
                if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
                {
                    m_Device->CreateRenderTargetView(pBackBuffer, NULL, m_mainRenderTargetView.GetAddressOf());
                    pBackBuffer->Release();
                }
                hooks::ogWndProc = (types::WNDPROC)SetWindowLongPtr(m_window, GWLP_WNDPROC, (LONG_PTR)WndProc);

                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO();
                io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
                ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get());
                ImGui_ImplWin32_Init(m_window);
                imgui_style::initStyle();
                g_Renderer->Menu.Initialize();

                PresentHooked = true;
            }
            else
            {
                return ogPresent(pSwapChain, SyncInterval, Flags);
            }
        }

        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
        if (sd.BufferDesc.Width != g_Renderer->ScreenSize.x || sd.BufferDesc.Height != g_Renderer->ScreenSize.y)
        {
            m_mainRenderTargetView.Reset();
            ID3D11Texture2D* pBackBuffer = nullptr;
            if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
            {
                m_Device->CreateRenderTargetView(pBackBuffer, NULL, m_mainRenderTargetView.GetAddressOf());
                pBackBuffer->Release();
            }

            g_Renderer->ScreenSize = { static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height) };
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        g_Renderer->OnPresent();
        ImGui::Render();

        m_Context->OMSetRenderTargets(1, m_mainRenderTargetView.GetAddressOf(), NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        return ogPresent(pSwapChain, SyncInterval, Flags);
    }

}