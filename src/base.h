#pragma once
#ifndef BASE_H
#define BASE_H

#include <pch.h>

#define WNDPROC_INDEX GWLP_WNDPROC

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

DWORD WINAPI MainThread(LPVOID lpReserved);
DWORD WINAPI ExitThread(LPVOID lpReserved);

namespace Base {
	bool Shutdown();
	bool Detach();

	namespace Data 
	{
		extern HMODULE					hModule;
		extern Present					oPresent;
		extern HWND						window;
		extern WNDPROC					oWndProc;
		extern ID3D11Device*			pDevice;
		extern ID3D11DeviceContext*		pContext;
		extern ID3D11RenderTargetView*	mainRenderTargetView;
		extern bool						ShowMenu;
		extern bool						InitImGui;
		extern bool						Detached;
		extern UINT						WmKeys[0xFF];
		extern std::unique_ptr
			<DirectX::PrimitiveBatch
			<DirectX::VertexPositionColor>
			>							primitiveBatch;
		extern std::unique_ptr
			<DirectX::SpriteBatch>		spriteBatch;
		extern std::unique_ptr
			<DirectX::SpriteFont>		spriteFont;
		extern std::unique_ptr
			<DirectX::BasicEffect>		basicEffect;
		extern std::unique_ptr
			<DirectX::CommonStates>		commonStates;


		namespace Keys
		{
			const UINT ToggleMenu = VK_INSERT;
			const UINT DetachDll =	VK_END;
		}
	}

	namespace Hooks 
	{
		bool Shutdown();
		HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}

#endif