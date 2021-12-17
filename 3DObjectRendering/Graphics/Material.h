#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Buffers/LightBuffer.h"

using namespace DirectX;

class Material
{
public:
	INT init(ID3D11Device* t_pD3DDevice, LPCWSTR t_textureName, LPCWSTR vertexFilePath, LPCWSTR pixelFilePath);
	void render(ID3D11DeviceContext* t_pD3DDeviceContext);
	void deInit();

	INT setMatrices(ID3D11DeviceContext* t_pD3DDeviceContext, XMFLOAT4X4 t_pWorldMatrix, XMFLOAT4X4 t_pViewMatrix, XMFLOAT4X4 t_pProjectionMatrix, XMFLOAT3 t_pCameraPosition);
	INT setLight(ID3D11DeviceContext* t_pD3DDeviceContext, const LightBuffer& t_light);

private:
	INT createVertexShader(ID3D11Device* t_pD3DDevice, LPCWSTR vertexFilePath);
	INT createPixelShader(ID3D11Device* t_pD3DDevice, LPCWSTR pixelFilePath);
	INT createInputLayout(ID3D11Device* t_pD3DDevice, ID3DBlob* t_pVertexShaderCode);
	INT createMatrixBuffer(ID3D11Device* t_pD3DDevice);
	INT createCameraBuffer(ID3D11Device* t_pD3DDevice);
	INT createLightBuffer(ID3D11Device* t_pD3DDevice);
	INT createTextureAndSampler(ID3D11Device* t_pD3DDevice, LPCWSTR t_textureName);

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

	ID3D11ShaderResourceView* m_pTexture = nullptr;
	ID3D11SamplerState* m_pSampler = nullptr;
	
	// My Buffers
	ID3D11Buffer* m_pMatrixBuffer = nullptr;
	ID3D11Buffer* m_pCameraBuffer = nullptr;
	ID3D11Buffer* m_pLightBuffer = nullptr;
	
	struct MatrixBuffer
	{
		XMFLOAT4X4 worldViewProjectionMatrix;
		XMFLOAT4X4 worldMatrix;
	};

	struct CameraBuffer
	{
		DirectX::XMFLOAT3 cameraPosition;
		FLOAT pad;
	};

};