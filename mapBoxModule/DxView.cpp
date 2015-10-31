#include "stdafx.h"

#include <d3d11.h>

#include <directxmath.h>
#include "mbgl\map\view.hpp"
#include "DxView.hpp"



namespace mbgl
{
	int getKeyMods()
	{
		int mods = 0;

		if (GetKeyState(VK_SHIFT) & (1 << 31))
			mods |= VK_SHIFT;
		if (GetKeyState(VK_CONTROL) & (1 << 31))
			mods |= VK_CONTROL;
		if (GetKeyState(VK_MENU) & (1 << 31))
			mods |= VK_MENU;
		if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & (1 << 31))
			mods |= VK_LWIN;

		return mods;
	}

	DxView::DxView(bool fullscreen) : fullscreen(fullscreen)
	{
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		pd3dDevice = nullptr;
		pImmediateContext = nullptr;
		pSwapChain = nullptr;
		pRenderTargetView = nullptr;
		pDepthStencil = nullptr;
		pDepthStencilView = nullptr;
		tracking = false;
	}


	DxView::~DxView()
	{
	}


	void DxView::initialize(mbgl::Map *map)
	{
		// Windows
		View::initialize(map);

		if (!fullscreen)
		{
		HRESULT hr = initWindow(GetModuleHandleW(NULL), SW_NORMAL);
		if (FAILED(hr))
			return;
		}
		else
		{
			HRESULT hr = initWindow(GetModuleHandleW(NULL), SW_MAXIMIZE);
			if (FAILED(hr))
				return;
		}

		InitDevice();


	}

	HRESULT DxView::initWindow(HINSTANCE hInstance, int nCmdShow)
	{
		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"SampleWindowClass";
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_APPLICATION);
		if (!RegisterClassEx(&wcex))
			return E_FAIL;

		// Create window
		hInst = hInstance;
		RECT rc = { 0, 0, 1024, 768 };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hwindow = CreateWindow(L"SampleWindowClass", L"DirectXTK Simple Sample", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
			this);
		if (!hwindow)
			return E_FAIL;

		ShowWindow(hwindow, nCmdShow);

		return S_OK;
	}

	HRESULT DxView::InitDevice()
	{
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(hwindow, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwindow;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		if (fullscreen)
		{
			sd.Windowed = FALSE;
		}
		else
		{
			sd.Windowed = TRUE;
		}
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pImmediateContext);
			if (SUCCEEDED(hr))
				break;

			//IDXGIFactory1* pFactory;
			//CreateDXGIFactory1(IID_IDXGIFactory1, (void**)&pFactory);
			//
			//IDXGIAdapter1* pAdapter;
			//pFactory->EnumAdapters1(0, (void**)&pAdapter);
			//D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &pd3dDevice, &featureLevel, &pImmediateContext);
			//pFactory->CreateSwapChain(pd3dDevice, &sd, &pSwapChain);
			//pSwapChain->

		}
		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
		if (FAILED(hr))
			return hr;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
		if (FAILED(hr))
			return hr;

		pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports(1, &vp);


		return S_OK;
	}

	void DxView::activate() {
		// no op
	}

	void DxView::deactivate() {
		//no-op
	}
	void DxView::notify() {
		PostMessage(hwindow, WM_NULL, 0, 0);
		run();
	}
	void DxView::invalidate(std::function<void()> render) {
		render();
		pSwapChain->Present(0, 0);

	}

	void DxView::onKey(DxView* window, int key, int scancode, int action, int mods) {
		DxView *view = window;

		if (action == 1) {
			switch (key) {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_TAB:
				view->map->toggleDebug();
				break;
			case 'X':
				if (!mods)
					view->map->resetPosition();
				break;
			case 'S':
				/*if (view->changeStyleCallback)
					view->changeStyleCallback();*/
				break;
			case 'R':
				if (!mods) {
					view->map->setDefaultTransitionDuration(std::chrono::milliseconds(300));
					if (view->map->hasClass("night")) {
						view->map->removeClass("night");
					}
					else {
						view->map->addClass("night");
					}
				}
				break;
			case 'N':
				if (!mods)
					view->map->resetNorth();
				break;
			}
		}
	}

	void DxView::onScroll(DxView* window, double xoffset, double yoffset) {
		DxView *view = window;
		double delta = yoffset * 40;

		bool isWheel = delta != 0 && std::fmod(delta, 4.000244140625) == 0;

		double absDelta = delta < 0 ? -delta : delta;
		double scale = 2.0 / (1.0 + std::exp(-absDelta / 100.0));

		// Make the scroll wheel a bit slower.
		if (!isWheel) {
			scale = (scale - 1.0) / 2.0 + 1.0;
		}

		// Zooming out.
		if (delta < 0 && scale != 0) {
			scale = 1.0 / scale;
		}

		view->map->scaleBy(scale, view->lastX, view->lastY);
	}
	void DxView::onResize(DxView* window, int width, int height) {

	}
	void DxView::onMouseClick(DxView* window, int button, int action, int modifiers) {

		if (button == WM_RBUTTONDOWN || button == WM_RBUTTONUP ||
			((button == WM_LBUTTONDOWN || button == WM_LBUTTONUP) && modifiers & VK_CONTROL)) {
			window->rotating = action == (button == WM_RBUTTONDOWN || button == WM_LBUTTONDOWN);
			window->map->setGestureInProgress(window->rotating);
		}
		else if ((button == WM_LBUTTONDOWN || button == WM_LBUTTONUP)) {
			window->tracking = action == WM_LBUTTONDOWN;
			window->map->setGestureInProgress(window->tracking);

			if (action == WM_RBUTTONUP || action == WM_LBUTTONUP) {
				double now = std::chrono::steady_clock::now().time_since_epoch().count();
				if (now - window->lastClick < 0.4 /* ms */) {
					if (modifiers & VK_SHIFT) {
						window->map->scaleBy(0.5, window->lastX, window->lastY, std::chrono::milliseconds(500));
					}
					else {
						window->map->scaleBy(2.0, window->lastX, window->lastY, std::chrono::milliseconds(500));
					}
				}
				window->lastClick = now;
			}
		}


	}
	void DxView::onMouseMove(DxView* window, double x, double y) {

	}



	LRESULT CALLBACK DxView::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		DxView* pointer = (DxView*)GetWindowLongPtrW(hWnd, 0);
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_NCCREATE:
		{
			CREATESTRUCTW* cs = (CREATESTRUCTW*)lParam;
			SetWindowLongPtrW(hWnd, 0, (LONG_PTR)cs->lpCreateParams);
			break;
		}

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_LBUTTONDOWN:
			pointer->onMouseClick(pointer, WM_LBUTTONDOWN, WM_LBUTTONDOWN, wParam);
			break;
		case WM_RBUTTONDOWN:
			pointer->onMouseClick(pointer, WM_RBUTTONDOWN, WM_RBUTTONDOWN, wParam);
			break;
		case WM_LBUTTONUP:
			pointer->onMouseClick(pointer, WM_LBUTTONUP, WM_LBUTTONUP, wParam);
			break;
		case WM_RBUTTONUP:
			pointer->onMouseClick(pointer, WM_RBUTTONUP, WM_RBUTTONUP, wParam);
			break;
		case WM_MOUSEMOVE:
			{
				const int x = LOWORD(lParam);
				const int y = HIWORD(lParam);
				pointer->onMouseMove(pointer, x, y);
			}
			break;
		case WM_SIZE:
			pointer->onResize(pointer, HIWORD(lParam), LOWORD(lParam));
			break;
		case WM_MOUSEHWHEEL:
		{
			// This message is only sent on Windows Vista and later
			// NOTE: The X-axis is inverted for consistency with OS X and X11.
			pointer->onScroll(pointer, -((SHORT)HIWORD(wParam) / (double)WHEEL_DELTA), 0.0);
			
		}
		break;
		case WM_MOUSEWHEEL:
		{
			pointer->onScroll(pointer, 0.0, (SHORT)HIWORD(wParam) / (double)WHEEL_DELTA);
			
		}
		break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			const int scancode = (lParam >> 16) & 0x1ff;
			pointer->onKey(pointer, wParam, scancode, 0, getKeyMods());
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			const int scancode = (lParam >> 16) & 0x1ff;
			pointer->onKey(pointer, wParam, scancode, 1, getKeyMods());
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;

	}

	void DxView::run()
	{
		WaitMessage();

		MSG msg;
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// Treat WM_QUIT as a close on all windows
				// While GLFW does not itself post WM_QUIT, other processes may post
				// it to this one, for example Task Manager
				PostQuitMessage(0);
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}

	}
}