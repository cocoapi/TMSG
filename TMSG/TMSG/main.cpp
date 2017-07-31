#ifndef UNICODE
#define UNICODE
#endif 

#include "Headers.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void initDevice(DXGI_SWAP_CHAIN_DESC&, HWND&);
HRESULT createDevice(D3D_FEATURE_LEVEL&, DXGI_SWAP_CHAIN_DESC&, IDXGISwapChain*, ID3D11Device*, D3D_FEATURE_LEVEL&, ID3D11DeviceContext*);
HRESULT initRTV(ID3D11RenderTargetView *, ID3D11Texture2D *);
void Render();

IDXGISwapChain* ppSwapChain = NULL;
ID3D11Device* ppDevice = NULL;
D3D_FEATURE_LEVEL FeatureLevel;
ID3D11DeviceContext* ppImmediateContext = NULL;
ID3D11RenderTargetView* pRenderTargetView = NULL;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"TMSG";

	WNDCLASS wc = {};
	DXGI_SWAP_CHAIN_DESC sd;
	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D *backBuf = NULL;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"TMSG",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,

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
	if (ERROR == createDevice(FeatureLevels, sd, ppSwapChain, ppDevice, FeatureLevel, ppImmediateContext)) {
		MessageBox(hwnd, L"CREATEDEVICE ERROR!!", L"ERROR", MB_OK);
		return 0;
	}
	if (ERROR == initRTV(pRenderTargetView, backBuf)) {
		MessageBox(hwnd, L"RENDERTAGETVIEW ERROR!!", L"ERROR", MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Render();
		}
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
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void initDevice(DXGI_SWAP_CHAIN_DESC& sd, HWND& hwnd) {
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

HRESULT initRTV(ID3D11RenderTargetView *RTV, ID3D11Texture2D *backBuf) {
	HRESULT hr;
	if (FAILED(hr = ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuf)))) {
		hr = ERROR;
		return hr;
	}
	if (FAILED(hr = ppDevice->CreateRenderTargetView(backBuf, 0, &RTV))) {
		hr = ERROR;
		return hr;
	}
	backBuf->Release();
	return hr;
}

void Render()
{
	// Just clear the backbuffer
	float ClearColor[4] = { 1.0f, 5.0f, 1.0f, 0.0f }; //red,green,blue,alpha
	ppImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	ppSwapChain->Present(0, 0);
}