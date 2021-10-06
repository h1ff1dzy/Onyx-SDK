#include <pch.h>
#include <fonts/Purista.h>
#define PRESENT_FN_IDX 8
float OnyxSDK::ScreenX = 0, OnyxSDK::ScreenY = 0;
float OnyxSDK::ScreenSX = 0, OnyxSDK::ScreenSY = 0;

void OnyxSDK::Init(IDXGISwapChain* _pSwapChain)
{
	try {
		pSwapChain = _pSwapChain;
		void const* shaderByteCode;
		size_t byteCodeLength;
		Base::Data::pContext->IAGetInputLayout(&InputLayout);
		Base::Data::spriteBatch.reset(new DirectX::SpriteBatch(Base::Data::pContext));
		Base::Data::spriteFont.reset(new DirectX::SpriteFont(Base::Data::pDevice, Fonts::Purista, sizeof(Fonts::Purista)));
		Base::Data::primitiveBatch.reset(new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(Base::Data::pContext));
		Base::Data::commonStates.reset(new DirectX::CommonStates(Base::Data::pDevice));
		Base::Data::basicEffect.reset(new DirectX::BasicEffect(Base::Data::pDevice));
		//DXGI_SWAP_CHAIN_DESC pSwapChainDesc;

		//pSwapChain->GetDesc(&pSwapChainDesc);


		RECT screen;
		GetWindowRect(Base::Data::window, &screen);
		ScreenX = (float)screen.left;
		ScreenY = (float)screen.top;
		ScreenSX = (float)screen.right - (float)screen.left;
		ScreenSY = (float)screen.bottom - (float)screen.top;

		Base::Data::basicEffect->SetWorld(DirectX::XMMatrixIdentity());
		Base::Data::basicEffect->SetView(DirectX::XMMatrixIdentity());
		Base::Data::basicEffect->SetProjection(DirectX::XMMatrixOrthographicOffCenterRH(0.0f, ScreenSX, ScreenSY, 0.0f, 0.0f, 1.0f));

		HRESULT hr = S_OK;

		if (FAILED(hr)) {
			Base::Detach();
		}

		Base::Data::pDevice->GetImmediateContext(&Base::Data::pContext);

		Base::Data::basicEffect->SetVertexColorEnabled(true);

		Base::Data::basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		hr = Base::Data::pDevice->CreateInputLayout(
			DirectX::VertexPositionColor::InputElements,
			DirectX::VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&InputLayout
		);

		if (FAILED(hr)) {
			Base::Detach();
		}
		stateSaver = new DXTKStateSaver;
	}
	catch (...) {
		Base::Detach();
	}
	isInitializated = true;
}

void OnyxSDK::Menu() {
	ImGui::Begin("ImGui Window");
	ImGui::Text("Test ImGUI Window");
	ImGui::End();
}

IDXGISwapChain* OnyxSDK::findSwapChain(PDWORD const dwCode)
{

	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;

	IDXGISwapChain* pSwapChain = NULL;

	DWORD64 dwSwapChainVFT = 0;
	IDXGISwapChain* pNewChain = NULL;

	SIZE_T iMemIndex;
	DWORD64 dwPtr = 0, dwOwner = 0;
	MEMORY_BASIC_INFORMATION mbi;

	D3D_FEATURE_LEVEL pFeatures[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC stSwapDesc = { 0 };
	stSwapDesc.BufferCount = 1;
	stSwapDesc.BufferDesc.Width = 640;
	stSwapDesc.BufferDesc.Height = 480;
	stSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	stSwapDesc.BufferDesc.RefreshRate.Numerator = 60;
	stSwapDesc.BufferDesc.RefreshRate.Denominator = 1;

	stSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	stSwapDesc.OutputWindow = GetForegroundWindow();
	stSwapDesc.SampleDesc.Count = 1;
	stSwapDesc.SampleDesc.Quality = 0;
	stSwapDesc.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		pFeatures,
		1,
		D3D11_SDK_VERSION,
		&stSwapDesc,
		&pSwapChain,
		&pDevice,
		NULL,
		&pContext
	);

	if (FAILED(hr)) {
		*dwCode = 1;
		goto __cleanup;
	}

	dwSwapChainVFT = *(DWORD64*)pSwapChain;

	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));

	for (DWORD64 dwMem = 0x10000; dwMem < 0x7FFFFFFFFFFF; dwMem = (DWORD64)mbi.BaseAddress + mbi.RegionSize) {
		if (VirtualQuery((LPCVOID)dwMem, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == FALSE)
			break;

		if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS || mbi.Protect & PAGE_GUARD)
			continue;

		for (iMemIndex = 0; iMemIndex < mbi.RegionSize; iMemIndex += 8) {
			dwPtr = *(DWORD64*)((BYTE*)mbi.BaseAddress + iMemIndex);

			if (dwPtr == dwSwapChainVFT) {

				dwOwner = (DWORD64)((PBYTE)mbi.BaseAddress + iMemIndex);

				if (dwOwner == (DWORD64)pSwapChain)
					continue;

				printf("Found swap chain at %llX\n", dwOwner);

				// Additional checks are needed here, but I removed them for some reason...
				// A variety of other things are also wrong with this function... it would be wise to get rid of this outside of simple test environments

				pNewChain = (IDXGISwapChain*)dwOwner;
			}
		}
	}

__cleanup:

	if (pSwapChain != NULL) {
		pSwapChain->Release();
		pContext->Release();
		pDevice->Release();
	}

	return pNewChain;
}

void OnyxSDK::DXBegin() {

	restoreState = FALSE;
	stateSaver->saveCurrentState(Base::Data::pContext);
	restoreState = TRUE;

	Base::Data::spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);

	Base::Data::basicEffect->Apply(Base::Data::pContext);
	Base::Data::pContext->IASetInputLayout(InputLayout);

	Base::Data::primitiveBatch->Begin();
}
void OnyxSDK::DXEnd() {
	Base::Data::primitiveBatch->End();
	Base::Data::spriteBatch->End();
	if (restoreState) stateSaver->restoreSavedState();
}

void OnyxSDK::DrawEmptyBoxTab_2(float x, float y, float x1, float y1, float from, float to, float px, DirectX::SimpleMath::Color color) {
	FillRGB(x, y, from, px, color);
	FillRGB(x + to, y, x1 - to, px, color);
	FillRGB(x + x1, y, px, y1, color);
	FillRGB(x, y + y1 - px, x1 + px, px, color);
}

void OnyxSDK::DrawString(std::wstring Format, ImVec2 position, DirectX::SimpleMath::Color color, float size, bool shadow, bool center) {
	DirectX::XMVECTOR colorRGB = { color.R() / 255, color.G() / 255, color.B() / 255, color.A() / 255 };


	DirectX::XMFLOAT2 pos = { position.x, position.y };
	DirectX::XMFLOAT2 posShadow = { position.x + 1, position.y + 1 };
	DirectX::XMVECTOR coloraShadow = { 0.f, 0.f, 0.f, 1 };
	DirectX::XMFLOAT2 origin(0, 0);


	if (center)
	{
		DirectX::XMVECTOR size = Base::Data::spriteFont->MeasureString(Format.c_str());
		float sizeX = DirectX::XMVectorGetX(size);
		float sizeY = DirectX::XMVectorGetY(size);
		origin = DirectX::XMFLOAT2(sizeX / 2, sizeY / 2);
	}

	if (shadow) Base::Data::spriteFont->DrawString(Base::Data::spriteBatch.get(), Format.c_str(), posShadow, coloraShadow, 0, origin, size * 0.64f, DirectX::SpriteEffects_None, 0);
	Base::Data::spriteFont->DrawString(Base::Data::spriteBatch.get(), Format.c_str(), pos, colorRGB, 0, origin, size * 0.64f, DirectX::SpriteEffects_None, 0);
}

void OnyxSDK::FillRGB(float x, float y, float x1, float y1, DirectX::SimpleMath::Color color) {
	DirectX::FXMVECTOR c1 = { x, y };
	DirectX::FXMVECTOR c2 = { x + x1, y };
	DirectX::FXMVECTOR c3 = { x + x1, y + y1 };
	DirectX::FXMVECTOR c4 = { x, y + y1 };
	DirectX::FXMVECTOR colora = { color.R() / 255.f, color.G() / 255.f, color.B() / 255.f, color.A() / 255.f };
	DirectX::VertexPositionColor d1(c1, colora);
	DirectX::VertexPositionColor d2(c2, colora);
	DirectX::VertexPositionColor d3(c3, colora);
	DirectX::VertexPositionColor d4(c4, colora);

	Base::Data::primitiveBatch->DrawQuad(d1, d2, d3, d4);
}

void OnyxSDK::DrawBox(float x, float y, float x1, float y1, DirectX::SimpleMath::Color color) {
	FillRGB(x, y, x1, y1, color);
}

void OnyxSDK::DrawEmptyBox(float x, float y, float x1, float y1, float px, DirectX::SimpleMath::Color color)
{
	FillRGB(x, y, x1, px, color);
	FillRGB(x + x1, y, px, y1, color);
	FillRGB(x, y + y1 - px, x1 + px, px, color);
	FillRGB(x, y, px, y1, color);
}
