#pragma once
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

class Direct3D
{
public:
	INT init(HWND t_hWnd, UINT t_width, UINT t_height, BOOL t_isFullscreen);
	void beginScene(FLOAT t_red, FLOAT t_green, FLOAT t_blue);
	void endScene();
	void deInit();

	ID3D11Device* getDevice() { return m_pD3DDevice; }
	ID3D11DeviceContext* getDeviceContext() { return m_pD3DDeviceContext; }

private:
	ID3D11Device* m_pD3DDevice = nullptr;
	ID3D11DeviceContext* m_pD3DDeviceContext = nullptr;
	IDXGISwapChain* m_pDXGISwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	ID3D11RasterizerState* m_pRasterizerState = nullptr;
	D3D11_VIEWPORT m_viewPort = {};
};

