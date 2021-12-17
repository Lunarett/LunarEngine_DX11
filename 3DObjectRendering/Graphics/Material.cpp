#include "Material.h"
#include "WICTextureLoader.h"
#include "Utilities.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

INT Material::init(ID3D11Device* pD3DDevice, LPCWSTR textureName, LPCWSTR vertexFilePath, LPCWSTR pixelFilePath)
{
	INT error = createVertexShader(pD3DDevice, vertexFilePath);
	CheckError(error);

	error = createPixelShader(pD3DDevice, pixelFilePath);
	CheckError(error);

	error = createMatrixBuffer(pD3DDevice);
	CheckError(error);

	error = createCameraBuffer(pD3DDevice);
	CheckError(error);

	error = createLightBuffer(pD3DDevice);
	CheckError(error);

	error = createTextureAndSampler(pD3DDevice, textureName);
	CheckError(error);

	return 0;
}

void Material::render(ID3D11DeviceContext* t_pD3DDeviceContext)
{
	t_pD3DDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	t_pD3DDeviceContext->PSSetSamplers(0, 1, &m_pSampler);

	t_pD3DDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	t_pD3DDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	t_pD3DDeviceContext->IASetInputLayout(m_pInputLayout);
}

void Material::deInit()
{
	safeRelease<ID3D11SamplerState>(m_pSampler);
	safeRelease<ID3D11ShaderResourceView>(m_pTexture);
	safeRelease<ID3D11Buffer>(m_pLightBuffer);
	safeRelease<ID3D11Buffer>(m_pMatrixBuffer);
	safeRelease<ID3D11InputLayout>(m_pInputLayout);
	safeRelease<ID3D11VertexShader>(m_pVertexShader);
	safeRelease<ID3D11PixelShader>(m_pPixelShader);
}

INT Material::setMatrices(ID3D11DeviceContext* t_pD3DDeviceContext, XMFLOAT4X4 t_pWorldMatrix, XMFLOAT4X4 t_pViewMatrix, XMFLOAT4X4 t_pProjectionMatrix, XMFLOAT3 t_pCameraPosition)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));
	unsigned int bufferNum;
	MatrixBuffer* matrixB;
	CameraBuffer* camB;



	// Setup Matrix Buffer Params
	hr = t_pD3DDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CheckFailed(hr, 75);

	XMMATRIX worldMatrix = XMLoadFloat4x4(&t_pWorldMatrix);
	XMMATRIX viewMatrix = XMLoadFloat4x4(&t_pViewMatrix);
	XMMATRIX projectionMatrix = XMLoadFloat4x4(&t_pProjectionMatrix);

	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * projectionMatrix;

	MatrixBuffer* buffer = reinterpret_cast<MatrixBuffer*>(mappedResource.pData);
	XMStoreFloat4x4(&(buffer->worldViewProjectionMatrix), wvpMatrix);
	XMStoreFloat4x4(&(buffer->worldMatrix), XMMatrixTranspose(worldMatrix));

	t_pD3DDeviceContext->Unmap(m_pMatrixBuffer, 0);

	t_pD3DDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);



	// Setup Camera Buffer Params
	hr = t_pD3DDeviceContext->Map(m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CheckFailed(hr, 74);

	camB = reinterpret_cast<CameraBuffer*>(mappedResource.pData);

	camB->cameraPosition = t_pCameraPosition;
	camB->pad = 0;

	t_pD3DDeviceContext->Unmap(m_pCameraBuffer, 0);
	bufferNum = 1;
	t_pD3DDeviceContext->VSSetConstantBuffers(bufferNum, 1, &m_pCameraBuffer);
}

INT Material::setLight(ID3D11DeviceContext* t_pD3DDeviceContext, const LightBuffer& t_light)
{
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(data));
	HRESULT hr = t_pD3DDeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	CheckFailed(hr, 88);

	LightBuffer* pLight = reinterpret_cast<LightBuffer*>(data.pData);
	*pLight = t_light;

	t_pD3DDeviceContext->Unmap(m_pLightBuffer, 0);
	t_pD3DDeviceContext->PSSetConstantBuffers(0, 1, &m_pLightBuffer);
	//return 0;
}

INT Material::createVertexShader(ID3D11Device* t_pD3DDevice, LPCWSTR vertexFilePath)
{
	ID3DBlob* pCompiledShaderCode = nullptr;
	HRESULT hr = D3DReadFileToBlob(vertexFilePath, &pCompiledShaderCode);
	CheckFailed(hr, 60);

	hr = t_pD3DDevice->CreateVertexShader(pCompiledShaderCode->GetBufferPointer(), pCompiledShaderCode->GetBufferSize(), nullptr, &m_pVertexShader);
	CheckFailed(hr, 62);

	INT error = createInputLayout(t_pD3DDevice, pCompiledShaderCode);
	CheckError(error);

	safeRelease<ID3DBlob>(pCompiledShaderCode);

	return 0;
}

INT Material::createPixelShader(ID3D11Device* t_pD3DDevice, LPCWSTR pixelFilePath)
{
	ID3DBlob* pCompiledShaderCode = nullptr;
	HRESULT hr = D3DReadFileToBlob(pixelFilePath, &pCompiledShaderCode);
	CheckFailed(hr, 64);

	hr = t_pD3DDevice->CreatePixelShader(pCompiledShaderCode->GetBufferPointer(), pCompiledShaderCode->GetBufferSize(), nullptr, &m_pPixelShader);
	CheckFailed(hr, 66);

	safeRelease<ID3DBlob>(pCompiledShaderCode);

	return 0;
}

INT Material::createInputLayout(ID3D11Device* t_pD3DDevice, ID3DBlob* t_pVertexShaderCode)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = getLength(layout);

	HRESULT hr = t_pD3DDevice->CreateInputLayout(layout, numElements, t_pVertexShaderCode->GetBufferPointer(),t_pVertexShaderCode->GetBufferSize(), &m_pInputLayout);
	CheckFailed(hr, 69);


	return 0;
}

INT Material::createMatrixBuffer(ID3D11Device* t_pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MatrixBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = t_pD3DDevice->CreateBuffer(&desc, nullptr, &m_pMatrixBuffer);
	CheckFailed(hr, 61);

	return 0;
}

INT Material::createCameraBuffer(ID3D11Device* t_pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(CameraBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = t_pD3DDevice->CreateBuffer(&desc, nullptr, &m_pCameraBuffer);
	CheckFailed(hr, 70);

	return 0;
}

INT Material::createLightBuffer(ID3D11Device* t_pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(LightBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = t_pD3DDevice->CreateBuffer(&desc, nullptr, &m_pLightBuffer);
	CheckFailed(hr, 67);

	return 0;
}

INT Material::createTextureAndSampler(ID3D11Device* t_pD3DDevice, LPCWSTR t_textureName)
{
	HRESULT hr = CreateWICTextureFromFile(t_pD3DDevice, t_textureName, nullptr, &m_pTexture);
	CheckFailed(hr, 63);

	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = t_pD3DDevice->CreateSamplerState(&desc, &m_pSampler);
	CheckFailed(hr, 65);

	return 0;
}
