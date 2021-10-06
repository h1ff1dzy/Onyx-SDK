#include <pch.h>
#include <base.h>

#pragma warning( disable : 26812 ) 

DWORD WINAPI MainThread(LPVOID lpThreadParam)
{
    Base::Data::hModule = (HMODULE)lpThreadParam;
    if (kiero::init(kiero::RenderType::Enum::D3D11) == kiero::Status::Success)
        kiero::bind(8, (void**)&Base::Data::oPresent, Base::Hooks::Present);
    return TRUE;
}

DWORD WINAPI ExitThread(LPVOID lpThreadParam)
{
    if (!Base::Data::Detached)
    {
        Base::Data::Detached = true;
        FreeLibraryAndExitThread(Base::Data::hModule, TRUE);
    }
    return TRUE;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        if (!Base::Data::Detached)
            CreateThread(nullptr, 0, ExitThread, hModule, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}

