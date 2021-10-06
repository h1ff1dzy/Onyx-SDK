#pragma once
#include <pch.h>
#include <base.h>
#include <DXTK/state.h>


class OnyxSDK {
private:
	bool restoreState = false;
	DXTKStateSaver* stateSaver;
	IDXGISwapChain* pSwapChain = NULL;
	bool isInitializated = false;
	void FillRGB(float x, float y, float x1, float y1, DirectX::SimpleMath::Color color);
	IDXGISwapChain* findSwapChain(PDWORD const pCode);
public:
	static float ScreenX, ScreenY;
	static float ScreenSX, ScreenSY;
	ID3D11InputLayout* InputLayout;
	void Init(IDXGISwapChain* pSwapChain);
	void DXBegin();
	void DXEnd();
	void Menu();
	void DrawEmptyBoxTab_2(float x, float y, float x1, float y1, float from, float to, float px, DirectX::SimpleMath::Color color);
	void DrawString(std::wstring Format, ImVec2 position, DirectX::SimpleMath::Color color, float size, bool shadow = false, bool center = false);
	void DrawBox(float x, float y, float x1, float y1, DirectX::SimpleMath::Color color);
	void DrawEmptyBox(float x, float y, float x1, float y1, float px, DirectX::SimpleMath::Color color);
};