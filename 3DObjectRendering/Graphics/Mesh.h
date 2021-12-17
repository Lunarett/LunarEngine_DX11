#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Vertex.h"

#pragma comment(lib, "assimp-vc141-mtd.lib")

class Mesh
{
public:
	Mesh();

	INT loadMesh(ID3D11Device* t_pD3DDevice, const CHAR* filename, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale);
	
	void update(FLOAT t_deltaTime);
	void render(ID3D11DeviceContext* t_pD3DDeviceContext);
	
	void setPosition(DirectX::XMVECTOR t_position);
	void setPosition(FLOAT x, FLOAT y, FLOAT z);
	void setRotation(DirectX::XMVECTOR t_rotation);
	void setRotation(FLOAT x, FLOAT y, FLOAT z);
	void setScale(DirectX::XMVECTOR t_scale);
	void setScale(FLOAT x, FLOAT y, FLOAT z);

	void adjustPosition(DirectX::XMVECTOR t_position);
	void adjustPosition(FLOAT x, FLOAT y, FLOAT z);
	void adjustRotation(DirectX::XMVECTOR t_rotation);
	void adjustRotation(FLOAT x, FLOAT y, FLOAT z);
	void adjustScale(DirectX::XMVECTOR t_scale);
	void adjustScale(FLOAT x, FLOAT y, FLOAT z);

	DirectX::XMFLOAT3 getPositionFloat3() { return m_positionFloat3; }
	DirectX::XMFLOAT3 getRotationFloat3() { return m_rotationFloat3; }
	DirectX::XMFLOAT3 getScaleFloat3() { return m_scaleFloat3; }
	
	DirectX::XMVECTOR getPositionVector() { return m_positionVector; }
	DirectX::XMVECTOR getRotationVector() { return m_rotationVector; }
	DirectX::XMVECTOR getScaleVector() { return m_scaleVector; }

	DirectX::XMFLOAT4X4 getWorldMatrix4x4() { return m_worldMatrix4x4; }
	DirectX::XMMATRIX getWorldMatrix() { return m_worldMatrix; }

	void deInit();

private:
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	
	INT createBuffers(Vertex vertices[], WORD indices[], ID3D11Device* t_pD3DDevice);
	void updateTransform();

	UINT m_vertexCount = 0;
	UINT m_vertexStride = 0;
	UINT m_indexCount = 0;

	DirectX::XMFLOAT4X4 m_worldMatrix4x4 = {};
	DirectX::XMMATRIX m_worldMatrix = {};
	
	DirectX::XMFLOAT3 m_positionFloat3;
	DirectX::XMVECTOR m_positionVector;
	DirectX::XMFLOAT3 m_rotationFloat3;
	DirectX::XMVECTOR m_rotationVector;
	DirectX::XMFLOAT3 m_scaleFloat3;
	DirectX::XMVECTOR m_scaleVector;
};

