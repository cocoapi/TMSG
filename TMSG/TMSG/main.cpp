#ifndef UNICODE
#define UNICODE
#endif 

#include "Headers.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool initDevice(DXGI_SWAP_CHAIN_DESC&, HWND&);
HRESULT createDevice(D3D_FEATURE_LEVEL&, DXGI_SWAP_CHAIN_DESC&, IDXGISwapChain*, ID3D11Device*, D3D_FEATURE_LEVEL&, ID3D11DeviceContext*);

IDXGISwapChain* ppSwapChain;
ID3D11Device* ppDevice;
D3D_FEATURE_LEVEL FeatureLevel;
ID3D11DeviceContext* ppImmediateContext;



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};
	DXGI_SWAP_CHAIN_DESC sd;
	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	initDevice(sd, hwnd);
	createDevice(FeatureLevels, sd, ppSwapChain, ppDevice, FeatureLevel, ppImmediateContext);

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool initDevice(DXGI_SWAP_CHAIN_DESC& sd, HWND& hwnd) {
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 640;
	sd.BufferDesc.Height = 480;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	return true;
}


HRESULT createDevice(
	D3D_FEATURE_LEVEL &FeatureLevels,
	DXGI_SWAP_CHAIN_DESC& sd,
	IDXGISwapChain* g_pSwapChain,
	ID3D11Device* g_pd3dDevice,
	D3D_FEATURE_LEVEL& FeatureLevel,
	ID3D11DeviceContext* g_pImmediateContext
) {
	HRESULT hr = S_OK;

	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_REFERENCE,
		NULL,
		0,
		&FeatureLevels,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pd3dDevice,
		&FeatureLevel,
		&g_pImmediateContext)))
	{
		hr = ERROR;
		return hr;
	}
	return hr;
}