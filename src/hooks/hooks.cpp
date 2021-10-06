#include <pch.h>
#include <base.h>

bool Base::Hooks::Shutdown()
{
	if (Data::InitImGui)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	kiero::shutdown();
	SetWindowLongPtr(Data::window, WNDPROC_INDEX, (LONG_PTR)Data::oWndProc);

	return true;
}