#include "Direct3D.h"
#include "Utilities.h"

INT Direct3D::init(HWND t_hWnd, UINT t_width, UINT t_height, BOOL t_isFullscreen)
{
	// Swap Chains
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.OutputWindow = t_hWnd;
	desc.Windowed = !t_isFullscreen;
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferDesc.Width = t_width;
	desc.BufferDesc.Height = t_height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.SampleDesc.Count = 1;

	// Create Device
	D3D_FEATURE_LEVEL supportedFeatureLevels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL choosenFeatureLevel = {};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0,
		supportedFeatureLevels, 6,
		D3D11_SDK_VERSION,
		&desc, &m_pDXGISwapChain, &m_pD3DDevice, &choosenFeatureLevel, &m_pD3DDeviceContext
	);
	CheckFailed(hr, 20);

	// Render Target View
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	CheckFailed(hr, 22);

	hr = m_pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	CheckFailed(hr, 24);

	safeRelease<ID3D11Texture2D>(pBackBuffer);

	// Depth Stencil Buffer
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
	depthStencilBufferDesc.Width = t_width;
	depthStencilBufferDesc.Height = t_height;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	
	hr = m_pD3DDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &pDepthStencilBuffer);
	CheckFailed(hr, 26);

	hr = m_pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
	CheckFailed(hr, 28);

	safeRelease<ID3D11Texture2D>(pDepthStencilBuffer);

	// Rasterize State
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	hr = m_pD3DDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);

	// View Port
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_viewPort.Width = t_width;
	m_viewPort.Height = t_height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	// Render Pipeline
	m_pD3DDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pD3DDeviceContext->RSSetState(m_pRasterizerState);
	m_pD3DDeviceContext->RSSetViewports(1, &m_viewPort);

	return 0;
}

void Direct3D::beginScene(FLOAT t_red, FLOAT t_green, FLOAT t_blue)
{
	const FLOAT backgroundColor[] = { t_red, t_green, t_blue, 1.0f };
	m_pD3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView, backgroundColor);

	m_pD3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0xffffffff);
}

void Direct3D::endScene()
{
	m_pDXGISwapChain->Present(0, 0);
}

void Direct3D::deInit()
{
	safeRelease<ID3D11RasterizerState>(m_pRasterizerState);
	safeRelease<ID3D11DepthStencilView>(m_pDepthStencilView);
	safeRelease<ID3D11RenderTargetView>(m_pRenderTargetView);
	safeRelease<ID3D11Device>(m_pD3DDevice);
	safeRelease<ID3D11DeviceContext>(m_pD3DDeviceContext);
	safeRelease<IDXGISwapChain>(m_pDXGISwapChain);
}
