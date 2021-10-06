#include <pch.h>
#include <base.h>

OnyxSDK	SDK;
D3D11_VIEWPORT viewport;

HRESULT __stdcall Base::Hooks::Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) 
{
	if (!Data::InitImGui)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Data::pDevice)))
		{
			Data::pDevice->GetImmediateContext(&Data::pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			Data::window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer = NULL;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);


			if (pBackBuffer == NULL)
				return Data::oPresent(pSwapChain, SyncInterval, Flags);

			Data::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &Data::mainRenderTargetView);
			pBackBuffer->Release();
			Data::oWndProc = (WNDPROC)SetWindowLongPtr(Data::window, GWLP_WNDPROC, (LONG_PTR)WndProc);


			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(Base::Data::window);
			ImGui_ImplDX11_Init(Base::Data::pDevice, Base::Data::pContext);
			SDK.Init(pSwapChain);
			Data::InitImGui = true;
		}

		else
			return Data::oPresent(pSwapChain, SyncInterval, Flags);
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Data::ShowMenu)
	{
		SDK.Menu();
	}

	ImGui::Render();

	Data::pContext->OMSetRenderTargets(1, &Data::mainRenderTargetView, NULL);

	SDK.DXBegin();
	{
		SDK.DrawString(L"OnyxSDK | h1ff#6024", { 10,10 }, { 228,12,90,255 }, 1, true, false);
	}
	SDK.DXEnd();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return Data::oPresent(pSwapChain, SyncInterval, Flags);
}