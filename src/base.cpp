#include <pch.h>
#include <base.h>

//	Data
HMODULE						Base::Data::hModule = (HMODULE)NULL;
Present						Base::Data::oPresent;
HWND						Base::Data::window		= NULL;
ID3D11Device*				Base::Data::pDevice		= NULL;
ID3D11DeviceContext*		Base::Data::pContext	= NULL;
WNDPROC						Base::Data::oWndProc;
ID3D11RenderTargetView*		Base::Data::mainRenderTargetView;
bool						Base::Data::ShowMenu	= true;
bool						Base::Data::InitImGui	= false;
bool						Base::Data::Detached	= false;
UINT						Base::Data::WmKeys[0xFF];
std::unique_ptr
<DirectX::SpriteBatch>		Base::Data::spriteBatch;
std::unique_ptr
<DirectX::SpriteFont>		Base::Data::spriteFont;
std::unique_ptr
<DirectX::PrimitiveBatch
	<DirectX::VertexPositionColor>
>							Base::Data::primitiveBatch;
std::unique_ptr
<DirectX::BasicEffect>		Base::Data::basicEffect;
std::unique_ptr
<DirectX::CommonStates>		Base::Data::commonStates;

bool Base::Shutdown()
{
	Hooks::Shutdown();
	return true;
}

bool Base::Detach()
{
	Base::Shutdown();
	CreateThread(nullptr, 0, ExitThread, Data::hModule, 0, nullptr);
	return true;
}