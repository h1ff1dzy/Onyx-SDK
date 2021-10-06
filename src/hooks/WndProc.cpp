#include <pch.h>
#include <base.h>


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Base::Hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
		Data::WmKeys[wParam] = uMsg;

	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
			case Data::Keys::ToggleMenu:
				Data::ShowMenu = !Data::ShowMenu;
				break;
			case Data::Keys::DetachDll:
				Base::Detach();
				break;
			//Detach
		}
	}
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	return CallWindowProc(Data::oWndProc, hWnd, uMsg, wParam, lParam);
}