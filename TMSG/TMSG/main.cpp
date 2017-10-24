#ifndef UNICODE
#define UNICODE
#endif 

#include "Headers.h"

#define CLIENTWIDTH 640
#define CLIENTHEIGHT 480

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void initDevice(DXGI_SWAP_CHAIN_DESC&, HWND&);
HRESULT createDevice(D3D_FEATURE_LEVEL&, DXGI_SWAP_CHAIN_DESC&, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL&, ID3D11DeviceContext**);
HRESULT initRTV(IDXGISwapChain*, ID3D11RenderTargetView **);
HRESULT depthStensilBufferandView(D3D11_TEXTURE2D_DESC *, ID3D11Texture2D*, ID3D11DepthStencilView*);
void makeView(D3D11_VIEWPORT *);
void Render();
void CleanupDevice();

IDXGISwapChain* ppSwapChain;
ID3D11Device* ppDevice;
ID3D11DeviceContext* ppImmediateContext;
ID3D11RenderTargetView* pRenderTargetView;
ID3D11Texture2D* pDepthStencilBuffer;
ID3D11DepthStencilView* pDepthStencilView;
D3D11_VIEWPORT vp;
D3D_FEATURE_LEVEL FeatureLevel;
D3D11_TEXTURE2D_DESC DepthStencilDesc;



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"TMSG";

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
		L"TMSG",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CLIENTWIDTH, CLIENTHEIGHT,

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
	if (E_FAIL == createDevice(FeatureLevels, sd, &ppSwapChain, &ppDevice, FeatureLevel, &ppImmediateContext)) {
		MessageBox(hwnd, L"CREATEDEVICE ERROR!!", L"ERROR", MB_OK);
		return 0;
	}
	if (E_FAIL == initRTV(ppSwapChain, &pRenderTargetView)) {
		MessageBox(hwnd, L"RENDERTAGETVIEW ERROR!!", L"ERROR", MB_OK);
		return 0;
	}
	
	if (E_FAIL == depthStensilBufferandView(&DepthStencilDesc, pDepthStencilBuffer, pDepthStencilView)) {
	MessageBox(hwnd, L"DEPTHSTENSIL ERROR!!", L"ERROR", MB_OK);
	return 0;
	}

	ppImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	ppImmediateContext->RSSetViewports(1, &vp);
	
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
			Render();
		}
	}

	CleanupDevice();
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

	case WM_LBUTTONDOWN:
		ppImmediateContext->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)(&Colors::Red));
		ppSwapChain->Present(0, 0);
		return 0;

	case WM_RBUTTONDOWN:
		ppImmediateContext->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)(&Colors::Blue));
		ppSwapChain->Present(0, 0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void initDevice(DXGI_SWAP_CHAIN_DESC& sd, HWND& hwnd) {
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = CLIENTWIDTH;
	sd.BufferDesc.Height = CLIENTHEIGHT;
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
	IDXGISwapChain** g_pSwapChain,
	ID3D11Device** g_pd3dDevice,
	D3D_FEATURE_LEVEL& FeatureLevel,
	ID3D11DeviceContext** g_pImmediateContext
) {
	HRESULT hr = S_OK;

	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_REFERENCE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		g_pSwapChain,
		g_pd3dDevice,
		&FeatureLevel,
		g_pImmediateContext)))
	{
		hr = ERROR;
		return hr;
	}
	return hr;
}

//initialize RenderTargetView
HRESULT initRTV(IDXGISwapChain *pSwapChain, ID3D11RenderTargetView **RTV) {
	HRESULT hr = S_OK;
	ID3D11Texture2D *backBuf;
	if (FAILED(hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuf)))) {
		hr = ERROR;
		return hr;
	}
	if (FAILED(hr = ppDevice->CreateRenderTargetView(backBuf, 0, &*RTV))) {
		hr = ERROR;
		return hr;
	}
	backBuf->Release();
	return hr;
}

//Make Depth-Stensil buffer and view
HRESULT depthStensilBufferandView(D3D11_TEXTURE2D_DESC *pdsd, ID3D11Texture2D* pdsb, ID3D11DepthStencilView* pdsv) {
	HRESULT hr = S_OK;
	pdsd->Width = 640;
	pdsd->Height = 480;
	pdsd->MipLevels = 1;
	pdsd->ArraySize = 1;
	pdsd->Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pdsd->SampleDesc.Count = 1;
	pdsd->SampleDesc.Quality = 0;
	pdsd->Usage = D3D11_USAGE_DEFAULT;
	pdsd->BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pdsd->CPUAccessFlags = 0;
	pdsd->MipLevels = 0;
	if (FAILED(hr = ppDevice->CreateTexture2D(pdsd, 0, &pdsb))) {
		hr = ERROR;
		return hr;
	}
	if (FAILED(hr = ppDevice->CreateDepthStencilView(pdsb, 0, &pdsv))) {
		hr = ERROR;
		return hr;
	}
	return hr;
}

//Make Viewport
void makeView(D3D11_VIEWPORT *vp) {
	vp->TopLeftX = 0.0f;
	vp->TopLeftY = 0.0f;
	vp->Width = static_cast<float>(CLIENTWIDTH);
	vp->Height = static_cast<float>(CLIENTHEIGHT);
	vp->MinDepth = 0.0f;
	vp->MaxDepth = 0.0f;
}
void Render()
{
	// Just clear the backbuffer
	ppImmediateContext->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)(&Colors::White));
	ppSwapChain->Present(0, 0);
}

void CleanupDevice()
{
	if (ppImmediateContext) ppImmediateContext->ClearState();

	if (pRenderTargetView) pRenderTargetView->Release();
	if (ppSwapChain) ppSwapChain->Release();
	if (ppImmediateContext) ppImmediateContext->Release();
	if (ppDevice) ppDevice->Release();
}