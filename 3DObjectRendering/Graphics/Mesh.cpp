#include <iostream>
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/vector3.h"
#include "assimp/cimport.h"
#include "Utilities.h"

using namespace std;
using namespace DirectX;

Mesh::Mesh()
{
	m_positionFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotationFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scaleFloat3 = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_positionVector = XMLoadFloat3(&m_positionFloat3);
	m_rotationVector = XMLoadFloat3(&m_rotationFloat3);
	m_scaleVector = XMLoadFloat3(&m_scaleFloat3);
}

INT Mesh::loadMesh(ID3D11Device* t_pD3DDevice, const CHAR* filename, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale)
{
	unsigned int flags = aiProcess_Triangulate | aiProcess_GenNormals;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, flags);


	if (!scene)
	{
		cout << "Failed to load Mesh file correctly!" << endl;
		return -1;
	}
	if (!scene->HasMeshes())
	{
		cout << "Scene has no mesh" << endl;
		return -1;
	}

	aiMesh* mesh = scene->mMeshes[0];
	vector<Vertex> vertices;
	vertices.resize(mesh->mNumVertices);

	// Calculate Vertices
	for (int v = 0; v < mesh->mNumVertices; v++)
	{
		// Position
		vertices[v].position = XMFLOAT3(
			mesh->mVertices[v].x,
			mesh->mVertices[v].y,
			mesh->mVertices[v].z
		);

		// Normal
		vertices[v].normal = XMFLOAT3(
			mesh->mNormals[v].x,
			mesh->mNormals[v].y,
			mesh->mNormals[v].z
		);

		// UV
		vertices[v].uv = XMFLOAT2(
			mesh->mTextureCoords[0][v].x,
			mesh->mTextureCoords[0][v].y
		);
	}
	m_vertexStride = sizeof(Vertex);
	m_vertexCount = vertices.size();

	vector<WORD> indices;
	indices.resize(mesh->mNumFaces * 3);
	m_indexCount = indices.size();


	// Calculate Indices
	for (int f = 0; f < mesh->mNumFaces; f++)
	{
		aiFace* face = mesh->mFaces + f;

		for (int i = 0; i < face->mNumIndices; i++)
		{
			indices[((3 * f) + i)] = *(face->mIndices + i);
		}
	}


	createBuffers(vertices.data(), indices.data(), t_pD3DDevice);
	
	setPosition(pos.x, pos.y, pos.z);
	setRotation(rot.x, rot.y, rot.z);
	setScale(scale.x, scale.y, scale.z);

	return 0;
}

void Mesh::update(FLOAT t_deltaTime)
{
}

void Mesh::render(ID3D11DeviceContext* t_pD3DDeviceContext)
{
	static UINT offset = 0;
	t_pD3DDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_vertexStride, &offset);
	t_pD3DDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	t_pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	t_pD3DDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::setPosition(DirectX::XMVECTOR t_position)
{
	XMStoreFloat3(&m_positionFloat3, t_position);
	m_positionVector = t_position;

	updateTransform();
}

void Mesh::setPosition(FLOAT x, FLOAT y, FLOAT z)
{
	m_positionFloat3 = XMFLOAT3(x, y, z);
	m_positionVector = XMLoadFloat3(&m_positionFloat3);

	updateTransform();
}

void Mesh::setRotation(DirectX::XMVECTOR t_rotation)
{
	m_rotationVector = t_rotation;
	XMStoreFloat3(&m_rotationFloat3, m_rotationVector);

	updateTransform();
}

void Mesh::setRotation(FLOAT x, FLOAT y, FLOAT z)
{
	m_rotationFloat3 = XMFLOAT3(x, y, z);
	m_rotationVector = XMLoadFloat3(&m_rotationFloat3);

	updateTransform();
}

void Mesh::setScale(DirectX::XMVECTOR t_scale)
{
	m_scaleVector = t_scale;
	XMStoreFloat3(&m_scaleFloat3, m_scaleVector);

	updateTransform();
}

void Mesh::setScale(FLOAT x, FLOAT y, FLOAT z)
{
	m_scaleFloat3 = XMFLOAT3(x, y, z);
	m_scaleVector = XMLoadFloat3(&m_scaleFloat3);

	updateTransform();
}

void Mesh::adjustPosition(DirectX::XMVECTOR t_position)
{
	m_positionVector += t_position;
	XMStoreFloat3(&m_positionFloat3, m_positionVector);

	updateTransform();
}

void Mesh::adjustPosition(FLOAT x, FLOAT y, FLOAT z)
{
	m_positionFloat3.x += x;
	m_positionFloat3.y += y;
	m_positionFloat3.z += z;

	m_positionVector = XMLoadFloat3(&m_positionFloat3);

	updateTransform();
}

void Mesh::adjustRotation(DirectX::XMVECTOR t_rotation)
{
	m_rotationVector += t_rotation;
	XMStoreFloat3(&m_rotationFloat3, m_rotationVector);

	updateTransform();
}

void Mesh::adjustRotation(FLOAT x, FLOAT y, FLOAT z)
{
	m_rotationFloat3.x += x;
	m_rotationFloat3.y += y;
	m_rotationFloat3.z += z;

	m_rotationVector = XMLoadFloat3(&m_rotationFloat3);

	updateTransform();
}

void Mesh::adjustScale(DirectX::XMVECTOR t_scale)
{
	m_scaleVector += t_scale;
	XMStoreFloat3(&m_rotationFloat3, m_scaleVector);

	updateTransform();
}

void Mesh::adjustScale(FLOAT x, FLOAT y, FLOAT z)
{
	m_scaleFloat3.x += x;
	m_scaleFloat3.y += y;
	m_scaleFloat3.z += z;

	m_scaleVector = XMLoadFloat3(&m_scaleFloat3);

	updateTransform();
}

void Mesh::deInit()
{
	safeRelease<ID3D11Buffer>(m_pVertexBuffer);
	safeRelease<ID3D11Buffer>(m_pIndexBuffer);
}

INT Mesh::createBuffers(Vertex vertices[], WORD indices[], ID3D11Device* t_pD3DDevice)
{
	// Create Vertex Buffer
	m_vertexStride = sizeof(Vertex);

	D3D11_BUFFER_DESC descVert = {};
	descVert.ByteWidth = m_vertexCount * m_vertexStride;
	descVert.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	descVert.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA dataVert = {};
	dataVert.pSysMem = vertices;

	HRESULT hrVert = t_pD3DDevice->CreateBuffer(&descVert, &dataVert, &m_pVertexBuffer);
	CheckFailed(hrVert, 30);


	// Create Index Buffer
	D3D11_BUFFER_DESC descInd = {};
	descInd.ByteWidth = m_indexCount * sizeof(WORD);
	descInd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	descInd.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA dataInd = {};
	dataInd.pSysMem = indices;

	HRESULT hrInd = t_pD3DDevice->CreateBuffer(&descInd, &dataInd, &m_pIndexBuffer);
	CheckFailed(hrInd, 32);
}

void Mesh::updateTransform()
{
	XMMATRIX translationMatrix = XMMatrixTranslation(m_positionFloat3.x, m_positionFloat3.y, m_positionFloat3.z);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_rotationFloat3.x, m_rotationFloat3.y, m_rotationFloat3.z);
	XMMATRIX scalingMatrix = XMMatrixScaling(m_scaleFloat3.x, m_scaleFloat3.y, m_scaleFloat3.z);

	XMStoreFloat4x4(&m_worldMatrix4x4, scalingMatrix * rotationMatrix * translationMatrix);
}
