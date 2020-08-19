// Example for D3D9 hook

// Include required libraries
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_INDEX_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION
#include "dependencies/Nuklear/nuklear.h"
#include "dependencies/Nuklear\demo\d3d11\nuklear_d3d11.h"
#include "dependencies/kiero.h"
#include <d3d11.h>
#include <dxgi.h>
#include <Windows.h>

// Initialize Variables
HWND gamewindow = GetActiveWindow();
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
nk_context* g_pNkContext;
int WindowHeight = 0;
int WindowWidth = 0;

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present oPresent;

void DoWindowValidation(void)
{
	RECT WindowRect;

	if (GetClientRect(gamewindow, &WindowRect))
	{
		if (WindowHeight == 0)
			WindowHeight = WindowRect.bottom - WindowRect.top;

		if (WindowWidth == 0)
			WindowWidth = WindowRect.right - WindowRect.left;

		int CurrentWidth = WindowRect.right - WindowRect.left;
		int CurrentHeight = WindowRect.bottom - WindowRect.top;

		if (WindowHeight != CurrentHeight || WindowWidth != CurrentWidth)
			GUIResize();
	}
}

// Declare the detour function
bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	DoWindowValidation();

	if (!init)
	{
		init = Initialize();
	}
	else
	{
		Start();

		MenuBackend();

		End();
	}
	
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}