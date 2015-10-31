#pragma once
#include <mbgl/mbgl.hpp>

namespace mbgl
{

	class DxView :
		public mbgl::View
	{
	public:
		DxView(bool fullscreen = false);
		~DxView();

		void initialize(mbgl::Map *map) override;
		HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
		HRESULT InitDevice();
		void activate() override;
		void deactivate() override;
		void notify() override;
		void invalidate(std::function<void()> render) override;
		void run();
		static LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

		static void onKey(DxView* window, int key, int scancode, int action, int mods);
		static void onScroll(DxView* window, double xoffset, double yoffset);
		static void onResize(DxView* window, int width, int height);
		static void onMouseClick(DxView* window, int button, int action, int modifiers);
		static void onMouseMove(DxView* window, double x, double y);


	private:
		const bool fullscreen;
		bool tracking;
		bool rotating;
		double lastClick;
		HINSTANCE hInst;
		HWND hwindow;

		D3D_DRIVER_TYPE                     driverType;
		D3D_FEATURE_LEVEL                   featureLevel;
		ID3D11Device*                       pd3dDevice;
		ID3D11DeviceContext*                pImmediateContext;
		IDXGISwapChain*                     pSwapChain;
		ID3D11RenderTargetView*             pRenderTargetView;
		ID3D11Texture2D*                    pDepthStencil;
		ID3D11DepthStencilView*             pDepthStencilView;

		double lastX, lastY;
	};

}